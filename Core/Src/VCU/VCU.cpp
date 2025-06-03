#pragma once
#include "VCU.hpp"


void VCU::initialize_state_machines(){
    GeneralStateMachine.add_state(GeneralStates::Connecting, "Connecting");
    GeneralStateMachine.add_state(GeneralStates::Operational, "Operational");
    GeneralStateMachine.add_state(GeneralStates::Fault, "Fault");
    GeneralStateMachine.set_initial_state(GeneralStates::Connecting);

    OperationalStateMachine.add_state(OperationalStates::Idle, "Idle");
    OperationalStateMachine.add_state(OperationalStates::Precharge, "Precharge");
    OperationalStateMachine.add_state(OperationalStates::EndOfRun, "EndOfRun");
    OperationalStateMachine.add_state(OperationalStates::Energized, "Energized");
    OperationalStateMachine.add_state(OperationalStates::Ready, "Ready");
    OperationalStateMachine.set_initial_state(OperationalStates::Idle);
    //id inventada en todas 
    HeapStateOrder Open_Contactors(0x0001,on_open_contactors,GeneralStateMachine,GeneralStates::Operational);
    HeapStateOrder Close_Contactors(0x0002,on_close_contactors,GeneralStateMachine,GeneralStates::Operational);
    HeapStateOrder Unbrake(0x0003,on_unbrake,GeneralStateMachine,GeneralStates::Operational);
    HeapStateOrder Brake(0x0004,on_brake,GeneralStateMachine,GeneralStates::Operational);

    GeneralStateMachine.add_transition(GeneralStates::Connecting, GeneralStates::Operational, [&](){
        return socket.is_connected() ...
    });

    GeneralStateMachine.add_transition(GeneralStates::Operational, GeneralStates::Fault, [&](){
        // !return socket.is_connected() y algo mas ...
    });

    GeneralStateMachine.add_transition(GeneralStates::Connecting, GeneralStates::Fault, [&](){
        // !return socket.is_connected() y algo mas ...
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


    OperationalStateMachine.add_transition(OperationalStates::Idle, OperationalStates::Precharge, [&](){
        return requested_close_contactors
    });

    OperationalStateMachine.add_transition(OperationalStates::Precharge, OperationalStates::Idle, [&](){
        return requested_open_contactors
    });

    OperationalStateMachine.add_transition(OperationalStates::Precharge, OperationalStates::Energyzed, [&](){
        // return veremos el que
    });

    OperationalStateMachine.add_transition(OperationalStates::Energyzed, OperationalStates::Idle, [&](){
        return requested_open_contactors
    });

    OperationalStateMachine.add_transition(OperationalStates::Idle, OperationalStates::EndOfRun, [&](){
        return requested_unbrake
    });

    OperationalStateMachine.add_transition(OperationalStates::Energyzed, OperationalStates::Ready, [&](){
        return requested_unbrake
    });

    OperationalStateMachine.add_transition(OperationalStates::Ready, OperationalStates::Energyzed, [&](){
        return requested_brake
    });

    HeapStateOrder Levitation_Active(0x0005,on_levitation_active,OperationalStateMachine,OperationalStates::Ready);
    HeapStateOrder Propulsion_Active(0x0006,on_propulsion_active,OperationalStateMachine,OperationalStates::Ready);
    HeapStateOrder Charging_LV_Battery(0x0007,on_charging_LV_battery,OperationalStateMachine,OperationalStates::Ready);
    HeapStateOrder Enable_Booster(0x0008,on_enable_booster,OperationalStateMachine,OperationalStates::Ready);
    HeapStateOrder Levitation_Inactive(0x0009,on_levitation_inactive,OperationalStateMachine,OperationalStates::Idle);
    HeapStateOrder Propulsion_Inactive(0x0010,on_propulsion_inactive,OperationalStateMachine,OperationalStates::Idle);
    HeapStateOrder Charging_LV_Battery_Inactive(0x0011,on_charging_LV_battery_inactive,OperationalStateMachine,OperationalStates::Idle);
    HeapStateOrder Disable_booster(0x0012,on_disable_booster,OperationalStateMachine,OperationalStates::Idle);

    OperationalStateMachine.add_enter_action([&](){
        requested_open_contactors = false;
    }, OperationalStates::Idle);

    OperationalStateMachine.add_enter_action([&](){
        requested_close_contactors = false;
    }, OperationalStates::Precharge);

    OperationalStateMachine.add_enter_action([&](){
        requested_unbrake = false;
    }, OperationalStates::EndOfRun);

    OperationalStateMachine.add_enter_action([&](){
        requested_unbrake = false;
    }, OperationalStates::Ready);

    OperationalStateMachine.add_enter_action([&](){
        requested_brake = false;
    }, OperationalStates::Energyzed);

    OperationalStateMachine.add_exit_action([&](){
        requested_levitation_active= false;
        requested_propulsion_active= false;
        requested_charging_LV_battery= false;
        requested_enable_booster= false;
    }, OperationalStates::Energyzed);

}

