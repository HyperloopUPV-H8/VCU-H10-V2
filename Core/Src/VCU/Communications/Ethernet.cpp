#pragma once
#include "Ethernet.hpp"

namespace Communications {
Ethernet::Ethernet(StateMachine* GeneralStateMachine, StateMachine* OperationalStateMachine) {
    PCU = (VCU_IP,pcu_port,PCU_IP,local_port);
    HVSCU = (VCU_IP,hvscu_port,HVSCU_IP,local_port);
    BMSL = (VCU_IP,bmsl_port,BMSL_IP,local_port);
    LCU = (VCU_IP,lcu_port,LCU_IP,local_port);
    BLCU = (VCU_IP,blcu_port,BLCU_IP,local_port);
    packets_endpoint =(local_ip, udp_port,control_station_ip, udp_port);
    
}

void Ethernet::initialize_state_orders(){
    Open_Contactors(0x0001,on_open_contactors,GeneralStateMachine,GeneralStates::Operational);
    Close_Contactors(0x0002,on_close_contactors,GeneralStateMachine,GeneralStates::Operational);
    Unbrake(0x0003,on_unbrake,GeneralStateMachine,GeneralStates::Operational);
    Brake(0x0004,on_brake,GeneralStateMachine,GeneralStates::Operational);
    Levitation_Active(0x0005,on_levitation_active,OperationalStateMachine,OperationalStates::Ready);
    Propulsion_Active(0x0006,on_propulsion_active,OperationalStateMachine,OperationalStates::Ready);
    Charging_LV_Battery(0x0007,on_charging_LV_battery,OperationalStateMachine,OperationalStates::Ready);
    Enable_Booster(0x0008,on_enable_booster,OperationalStateMachine,OperationalStates::Ready);
    Levitation_Inactive(0x0009,on_levitation_inactive,OperationalStateMachine,OperationalStates::Idle);
    Propulsion_Inactive(0x0010,on_propulsion_inactive,OperationalStateMachine,OperationalStates::Idle);
    Charging_LV_Battery_Inactive(0x0011,on_charging_LV_battery_inactive,OperationalStateMachine,OperationalStates::Idle);
    Disable_booster(0x0012,on_disable_booster,OperationalStateMachine,OperationalStates::Idle);
}
};