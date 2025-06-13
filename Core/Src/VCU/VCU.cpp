#include "VCU/VCU.hpp"


VCU::VCU(){
    initialize_state_machines();
    STLIB::start(ethernet.local_mac,ethernet.VCU_IP, "255.255.0.0","192.168.1.1",UART::uart2);
    ethernet = Communications::Ethernet(&GeneralStateMachine, &OperationalStateMachine);
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

    GeneralStateMachine.add_enter_action([&](){
        leds.leds_connecting();
    }, GeneralStates::Connecting);

    GeneralStateMachine.add_enter_action([&](){
        leds.leds_operational();
    }, GeneralStates::Operational);

    GeneralStateMachine.add_enter_action([&](){
        leds.leds_fault();
    }, GeneralStates::Fault);



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
        return ethernet.requested_brake;
    });

    OperationalStateMachine.add_transition(OperationalStates::Energyzed, OperationalStates::Ready, [&](){
        return ethernet.requested_unbrake;
    });

    //añadir transiciones


    ProtectionManager::link_state_machine(GeneralStateMachine, GeneralStates::Fault);
    ProtectionManager::add_standard_protections();
    ethernet.initialize_state_orders();

}

