#include "VCU/VCU.hpp"

void VCU::init(){
    state_machine = new VCU_SM();
    general_state = &state_machine->GeneralStateMachine.current_state;
    operational_state = &state_machine->OperationalStateMachine.current_state;

    ProtectionManager::add_standard_protections();
    ProtectionManager::initialize();
    ProtectionManager::link_state_machine(state_machine->GeneralStateMachine, VCU_SM::GeneralStates::Fault);
	ProtectionManager::set_id(Boards::ID::VCU);

    Comms::init();
}

void VCU::start(){
    //state_machine->actuators.init();
    //state_machine->brakes.init();

    Comms::actuators->init();
    Comms::brakes->init();

    Comms::start();
}

void VCU::send_packets(){
    if(Comms::packet_sending){
        Comms::send_packets();
        Comms::packet_sending = false;
    }
}

void VCU::update(){
    state_machine->GeneralStateMachine.check_transitions();
    send_packets();
    ProtectionManager::check_protections();
}