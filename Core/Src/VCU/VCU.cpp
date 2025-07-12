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

void VCU::read_sensors(){
    if(Comms::reading_sensors){
        Comms::read_sensors();
        Comms::reading_sensors = false;
    }
}

void VCU::update(){
    
    state_machine->GeneralStateMachine.check_transitions();
    state_machine->OperationalStateMachine.check_transitions();

    Comms::update();
    read_sensors();
    send_packets();
    ProtectionManager::check_protections();
}