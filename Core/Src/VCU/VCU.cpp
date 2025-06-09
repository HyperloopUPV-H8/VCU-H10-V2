#include "VCU/VCU.hpp"


VCU::VCU(){
    initialize_state_machines();
}

void VCU::initialize_state_machines(){
    GeneralStateMachine= StateMachine(GeneralStates::Connecting);
    GeneralStateMachine.add_state(GeneralStates::Operational);
    GeneralStateMachine.add_state(GeneralStates::Fault);
    
    OperationalStateMachine=StateMachine(OperationalStates::Idle);
    OperationalStateMachine.add_state(OperationalStates::EndOfRun);
    OperationalStateMachine.add_state(OperationalStates::Energyzed);
    OperationalStateMachine.add_state(OperationalStates::Ready);
    
    //id inventada en todas 
    // HeapStateOrder Open_Contactors(0x0001,on_open_contactors,GeneralStateMachine,GeneralStates::Operational);
    // HeapStateOrder Close_Contactors(0x0002,on_close_contactors,GeneralStateMachine,GeneralStates::Operational);
    // HeapStateOrder Unbrake(0x0003,on_unbrake,GeneralStateMachine,GeneralStates::Operational);
    // HeapStateOrder Brake(0x0004,on_brake,GeneralStateMachine,GeneralStates::Operational);

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

