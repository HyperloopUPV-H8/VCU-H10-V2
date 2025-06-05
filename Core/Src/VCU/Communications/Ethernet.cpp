#include "VCU/Communcations/Ethernet.hpp"

namespace Communications {


    std::vector<HeapPacket*> Ethernet::packets{};
    std::vector<Ethernet::OrderTriggers> Ethernet::order_triggers{};


    bool Ethernet::requested_unbrake = false;
    bool Ethernet::requested_open_contactors = false;
    bool Ethernet::requested_close_contactors = false;
    bool Ethernet::requested_brake = false;

    bool Ethernet::HVSCU_requested_close_contactors = false;
    bool Ethernet::HVSCU_requested_open_contactors = false;
    bool Ethernet::HVSCU_requested_start_charging = false;
    bool Ethernet::HVSCU_requested_stop_charging = false;

    bool Ethernet::BMSL_requested_start_charging = false;
    bool Ethernet::BMSL_requested_stop_charging = false;

    bool Ethernet::PCU_requested_stop_dynamic_run = false;
    bool Ethernet::PCU_requested_start_dynamic_run = false;

    bool Ethernet::LCU_requested_stop_vertical_levitation = false;
    bool Ethernet::LCU_requested_start_vertical_levitation = false;
    bool Ethernet::LCU_requested_stop_horizontal_levitation = false;
    bool Ethernet::LCU_requested_start_horizontal_levitation = false;

    bool Ethernet::BLCU_requested_upload_code = false;
    bool Ethernet::BLCU_requested_reset_vehicle = false;

Ethernet::Ethernet(StateMachine* GeneralStateMachine, StateMachine* OperationalStateMachine) {
    this->GeneralStateMachine = GeneralStateMachine;
    this->OperationalStateMachine = OperationalStateMachine;
    
    Control_station=(VCU_IP,local_port,control_station_ip,controlstation_port);

    Socket_PCU = (VCU_IP,pcu_port,PCU_IP,local_port);
    Socket_HVSCU = (VCU_IP,hvscu_port,HVSCU_IP,local_port);
    Socket_BMSL = (VCU_IP,bmsl_port,BMSL_IP,local_port);
    Socket_LCU = (VCU_IP,lcu_port,LCU_IP,local_port);
    Socket_BLCU = (VCU_IP,blcu_port,BLCU_IP,local_port);

    packets_endpoint =(local_ip, udp_controlstation_port,control_station_ip, udp_port);
    PCU_UDP = (local_ip, pcu_udp_port, PCU_IP, udp_port);
    HVSU_UDP = (local_ip, hvcu_udp_port, HVSU_IP, udp_port);
    BMSL_UDP = (local_ip, bmsl_udp_port, BMSL_IP, udp_port);
    LCU_UDP = (local_ip, lcu_udp_port, LCU_IP, udp_port);
    BLCU_UDP = (local_ip, blcu_udp_port, BLCU_IP, udp_port);
    
    // HVSCU orders:
    order_triggers.push_back({ Boards::HVSCU,
        &HVSCU_requested_close_contactors,
        &CloseContactors });
    order_triggers.push_back({ Boards::HVSCU,
            &HVSCU_requested_open_contactors,
            &OpenContactors });
    order_triggers.push_back({ Boards::HVSCU,
            &HVSCU_requested_start_charging,
            &StartChargingHV });
    order_triggers.push_back({ Boards::HVSCU,
            &HVSCU_requested_stop_charging,
            &StopChargingHV });

   // BMSL orders:
   order_triggers.push_back({ Boards::BMSL,
    &BMSL_requested_start_charging,
    &StartChargingLV });
    order_triggers.push_back({ Boards::BMSL,
        &BMSL_requested_stop_charging,
        &StopChargingLV });

    // PCU orders:
    order_triggers.push_back({ Boards::PCU,
        &PCU_requested_stop_dynamic_run,
        &StopDynamicRun });
    order_triggers.push_back({ Boards::PCU,
        &PCU_requested_start_dynamic_run,
        &StartDynamicRun });

    // LCU orders:
    order_triggers.push_back({ Boards::LCU,
        &LCU_requested_stop_vertical_levitation,
        &StopVerticalLevitation });
    order_triggers.push_back({ Boards::LCU,
        &LCU_requested_start_vertical_levitation,
        &StartVerticalLevitation });
    order_triggers.push_back({ Boards::LCU,
        &LCU_requested_stop_horizontal_levitation,
        &StopHorizontalLevitation });
    order_triggers.push_back({ Boards::LCU,
        &LCU_requested_start_horizontal_levitation,
        &StartHorizontalLevitation });

    // BLCU orders:
    order_triggers.push_back({ Boards::BLCU,
        &BLCU_requested_upload_code,
        &UploadCode });
    order_triggers.push_back({ Boards::BLCU,
        &BLCU_requested_reset_vehicle,
        &ResetVehicle });
}

void Ethernet::update(){
    for(const auto& order_trigger : order_triggers){
        if(*order_trigger.flag){
            if(order_trigger.board == Boards::HVSCU){
                Socket_HVSCU.send_order(order_trigger.order);
            }else if(order_trigger.board == Boards::BMSL){
                Socket_BMSL.send_order(order_trigger.order);
            }else if(order_trigger.board == Boards::PCU){
                Socket_PCU.send_order(order_trigger.order);
            }else if(order_trigger.board == Boards::LCU){
                Socket_LCU.send_order(order_trigger.order);
            }else if(order_trigger.board == Boards::BLCU){
                Socket_BLCU.send_order(order_trigger.order);
            }
            *order_trigger.flag = false; // Resetear la flag
        }
    }
}

bool Ethernet::connected(){
    return Socket_PCU.is_connected() && 
    Socket_HVSCU.is_connected() && 
    Socket_BMSL.is_connected() && 
    Socket_LCU.is_connected() && 
    Socket_BLCU.is_connected() &&
    Control_station.is_connected();
}

void Ethernet::initialize_state_orders(){
    Open_Contactors(VCU_ID::VCU_Open_contactors, &on_open_contactors, GeneralStateMachine, GeneralStates::Operational);
    Close_Contactors(VCU_ID::VCU_Close_contactors, &on_close_contactors, GeneralStateMachine, GeneralStates::Operational);
    Unbrake(VCU_ID::Unbrake, &on_unbrake, GeneralStateMachine, GeneralStates::Operational);
    Brake(VCU_ID::Brake, &on_brake, GeneralStateMachine, GeneralStates::Operational);
    Levitation_Active(VCU_ID::Levitation_active, &on_levitation_active, OperationalStateMachine, OperationalStates::Ready);
    Propulsion_Active(VCU_ID::Propulsion_active, &on_propulsion_active, OperationalStateMachine, OperationalStates::Ready);
    Charging_LV_Battery(VCU_ID::Charging_LV_battery, &on_charging_LV_battery, OperationalStateMachine, OperationalStates::Ready);
    Enable_Booster(VCU_ID::Enable_booster, &on_enable_booster, OperationalStateMachine, OperationalStates::Ready);
    Levitation_Inactive(VCU_ID::Levitation_inactive, &on_levitation_inactive, OperationalStateMachine, OperationalStates::Idle);
    Propulsion_Inactive(VCU_ID::Propulsion_inactive, &on_propulsion_inactive, OperationalStateMachine, OperationalStates::Idle);
    Charging_LV_Battery_Inactive(VCU_ID::Charging_LV_battery_inactive, &on_charging_LV_battery_inactive, OperationalStateMachine, OperationalStates::Idle);
    Disable_booster(VCU_ID::Disable_booster, &on_disable_booster, OperationalStateMachine, OperationalStates::Idle);
}
};