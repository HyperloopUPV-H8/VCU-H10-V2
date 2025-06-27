#include "VCU/VCU.hpp"

void VCU::init(){
    state_machine = new VCU_SM();
    VCU_state = &state_machine->GeneralStateMachine.current_state;

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

void VCU::update(){
    state_machine->GeneralStateMachine.check_transitions();
    ProtectionManager::check_protections();
}