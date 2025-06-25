#include "VCU/VCU.hpp"



StateMachine VCU::GeneralStateMachine;
StateMachine VCU::OperationalStateMachine;

VCU::VCU()
    : leds(Pinout::led_operational_pin, Pinout::led_fault_pin, Pinout::led_can_pin, Pinout::led_sleep_pin, Pinout::led_flash_pin)
    , Actuators(
        Pinout::regulator_1_in_pin,
        Pinout::regulator_2_in_pin,
        Pinout::regulator_1_out_pin,
        Pinout::regulator_2_out_pin,
        Pinout::Pump_c1_pin,
        Pinout::Pump_c2_pin,
        Pinout::Flow1_pin,
        Pinout::Flow2_pin,
        Pinout::SDC_pin,
        Pinout::Presion_alta_pin,
        Pinout::Presion_regulador_pin,
        Pinout::Presion_frenos_pin,
        Pinout::Presion_capsula_pin
    )
    , Brakes(
        Pinout::Brake_pin,
        Pinout::reed1_pin,
        Pinout::reed2_pin,
        Pinout::reed3_pin,
        Pinout::reed4_pin,
        Pinout::reed5_pin,
        Pinout::reed6_pin,
        Pinout::reed7_pin,
        Pinout::reed8_pin,
        Pinout::Tape_pin
    )
    , ethernet(&GeneralStateMachine, &OperationalStateMachine, &Actuators, &Brakes)
{
    initialize_state_machines();
    ethernet.initialize_state_orders();
    STLIB::start(ethernet.local_mac,ethernet.VCU_IP, "255.255.0.0","192.168.1.1",UART::uart2);
    Actuators.init();
    Brakes.init();

    Time::register_low_precision_alarm(16, [&]() {
        Brakes.read_reeds();
        Actuators.read_regulators();
        Actuators.read_pressure();
        Actuators.read_flow();
        Actuators.read_sdc();
    });
        
}



void VCU::initialize_state_machines(){
    GeneralStateMachine= StateMachine(GeneralStates::Connecting);
    GeneralStateMachine.add_state(GeneralStates::Operational);
    GeneralStateMachine.add_state(GeneralStates::Fault);
    
    OperationalStateMachine=StateMachine(OperationalStates::Idle);
    OperationalStateMachine.add_state(OperationalStates::EndOfRun);
    OperationalStateMachine.add_state(OperationalStates::Energyzed);
    OperationalStateMachine.add_state(OperationalStates::Ready);
    

    GeneralStateMachine.add_transition(GeneralStates::Connecting, GeneralStates::Operational, [&](){
        return ethernet.connected();
    });

    GeneralStateMachine.add_transition(GeneralStates::Operational, GeneralStates::Fault, [&](){
        return !ethernet.connected();
    });

    GeneralStateMachine.add_transition(GeneralStates::Connecting, GeneralStates::Fault, [&](){
        return !ethernet.connected(); // y algo mas para que no se vaya a fault al principio
    });

    GeneralStateMachine.add_transition(GeneralStates::Operational, GeneralStates::Fault, [&](){
        return (((Brakes.reed1== PinState::ON || Brakes.reed2==PinState::ON || Brakes.reed3==PinState::ON ||
               Brakes.reed4==PinState::ON || Brakes.reed5==PinState::ON || Brakes.reed6==PinState::ON ||
               Brakes.reed7==PinState::ON || Brakes.reed8==PinState::ON) && Brakes.Active_brakes )&& (!Brakes.breaks_first_time));
    });

    GeneralStateMachine.add_transition(GeneralStates::Connecting, GeneralStates::Fault, [&](){
        return (((Brakes.reed1== PinState::ON || Brakes.reed2==PinState::ON || Brakes.reed3==PinState::ON ||
               Brakes.reed4==PinState::ON || Brakes.reed5==PinState::ON || Brakes.reed6==PinState::ON ||
               Brakes.reed7==PinState::ON || Brakes.reed8==PinState::ON) && Brakes.Active_brakes )&& (!Brakes.breaks_first_time));
    });

    GeneralStateMachine.add_transition(GeneralStates::Operational, GeneralStates::Fault, [&](){
        return (!Actuators.Sdc);
    });

    GeneralStateMachine.add_transition(GeneralStates::Connecting, GeneralStates::Fault, [&](){
        return (!Actuators.Sdc);
    });

    GeneralStateMachine.add_enter_action([&](){
        leds.leds_connecting();
    }, GeneralStates::Connecting);

    GeneralStateMachine.add_enter_action([&](){
        leds.leds_operational();
    }, GeneralStates::Operational);

    GeneralStateMachine.add_enter_action([&](){
        leds.leds_fault();
        HAL_Delay(100);
        Brakes.brake();
    }, GeneralStates::Fault);

//el sdc tambien manda a fault?

    OperationalStateMachine.add_transition(OperationalStates::Idle, OperationalStates::Energyzed, [&](){
        return ethernet.requested_close_contactors;
    });

    OperationalStateMachine.add_transition(OperationalStates::Idle, OperationalStates::EndOfRun, [&](){
        return ethernet.requested_end_of_run;
    });

    OperationalStateMachine.add_transition(OperationalStates::Energyzed, OperationalStates::Idle,  [&](){
        return ethernet.requested_open_contactors;
        
    });

    OperationalStateMachine.add_transition(OperationalStates::Ready, OperationalStates::Energyzed, [&](){
        return Brakes.Active_brakes;
    });

    OperationalStateMachine.add_transition(OperationalStates::Energyzed, OperationalStates::Ready, [&](){
        return (!Brakes.Active_brakes);
    });

    OperationalStateMachine.add_enter_action([&]() {
        ethernet.requested_close_contactors = false;
    }, OperationalStates::Energyzed);

    

    OperationalStateMachine.add_enter_action([&]() {
        ethernet.requested_end_of_run = false;
    }, OperationalStates::EndOfRun);


    OperationalStateMachine.add_enter_action([&]() {
        ethernet.requested_open_contactors = false;
    }, OperationalStates::Idle);
    



    ProtectionManager::link_state_machine(GeneralStateMachine, GeneralStates::Fault);
    ProtectionManager::add_standard_protections();
    ethernet.initialize_state_orders();

}

void VCU::update(){
    STLIB::update();
    ethernet.update();

}

