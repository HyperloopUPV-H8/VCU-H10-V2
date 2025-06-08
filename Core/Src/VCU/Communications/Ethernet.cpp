#include "VCU/Communcations/Ethernet.hpp"

namespace Communications {


    
    // std::vector<Ethernet::OrderTriggers> Ethernet::order_triggers{};

    Flags_ready Ethernet::flags_ready{};


Ethernet::Ethernet(StateMachine* GeneralStateMachine, StateMachine* OperationalStateMachine) {
    this->GeneralStateMachine = GeneralStateMachine;
    this->OperationalStateMachine = OperationalStateMachine;
    
    Control_station=ServerSocket(VCU_IP,local_port);

    Socket_PCU = Socket(VCU_IP,pcu_port,PCU_IP,local_port);
    Socket_HVSCU = Socket(VCU_IP,hvscu_port,HVSCU_IP,local_port);
    Socket_BMSL = Socket(VCU_IP,bmsl_port,BMSL_IP,local_port);
    Socket_LCU = Socket(VCU_IP,lcu_port,LCU_IP,local_port);
    Socket_BLCU = Socket(VCU_IP,blcu_port,BLCU_IP,local_port);

    packets_endpoint =DatagramSocket(VCU_IP, udp_controlstation_port,control_station_ip, udp_port);
    // PCU_UDP = (VCU_IP, pcu_udp_port, PCU_IP, udp_port);
    // HVSU_UDP = (VCU_IP, hvcu_udp_port, HVSU_IP, udp_port);
    // BMSL_UDP = (VCU_IP, bmsl_udp_port, BMSL_IP, udp_port);
    // LCU_UDP = (VCU_IP, lcu_udp_port, LCU_IP, udp_port);
    // BLCU_UDP = (VCU_IP, blcu_udp_port, BLCU_IP, udp_port);

    Socket_to_board[Boards::PCU] = &Socket_PCU;
    Socket_to_board[Boards::HVSCU] = &Socket_HVSCU;
    Socket_to_board[Boards::BMSL] = &Socket_BMSL;
    Socket_to_board[Boards::LCU] = &Socket_LCU;
    Socket_to_board[Boards::BLCU] = &Socket_BLCU;
    //falta la blcu

    Open_Contactors=HeapStateOrder(Orders_id::VCU_Open_contactors, &on_open_contactors, GeneralStateMachine, GeneralStates::Operational);
    Close_Contactors=HeapStateOrder(Orders_id::VCU_Close_contactors, &on_close_contactors, GeneralStateMachine, GeneralStates::Operational);
    Unbrake=HeapStateOrder(Orders_id::Unbrake, &on_unbrake, GeneralStateMachine, GeneralStates::Operational);
    Brake=HeapStateOrder(Orders_id::Brake, &on_brake, GeneralStateMachine, GeneralStates::Operational);

    Levitation_Active=HeapStateOrder(Orders_id::Levitation_active, &on_levitation_active, OperationalStateMachine, OperationalStates::Ready);
    Propulsion_Active=HeapStateOrder(Orders_id::Propulsion_active, &on_propulsion_active, OperationalStateMachine, OperationalStates::Ready);
    Charging_LV_Battery=HeapStateOrder(Orders_id::Charging_LV_battery, &on_charging_LV_battery, OperationalStateMachine, OperationalStates::Ready);
    Enable_Booster=HeapStateOrder(Orders_id::Enable_booster, &on_enable_booster, OperationalStateMachine, OperationalStates::Ready);
    Levitation_Inactive=HeapStateOrder(Orders_id::Levitation_inactive, &on_levitation_inactive, OperationalStateMachine, OperationalStates::Idle);
    Propulsion_Inactive=HeapStateOrder(Orders_id::Propulsion_inactive, &on_propulsion_inactive, OperationalStateMachine, OperationalStates::Idle);
    Charging_LV_Battery_Inactive=HeapStateOrder(Orders_id::Charging_LV_battery_inactive, &on_charging_LV_battery_inactive, OperationalStateMachine, OperationalStates::Idle);
    Disable_booster=HeapStateOrder(Orders_id::Disable_booster, &on_disable_booster, OperationalStateMachine, OperationalStates::Idle);


}

void Ethernet::send_order(Boards board, HeapStateOrder* Order,Orders_id id){//Lo del id me imagino que se podra quitar 
    if(Order.ip==control_station_ip){//en pruebas esto!!
        Socket_to_board[board]->send_order(Order);
        id_to_timeout[id]=Time::set_timeout(5000,[&](){
            //InfoWarning("Timeout for order %d to board %s", id, Board_to_ip[board].to_string().c_str());
        })
    }if(Order.ip==Board_to_ip[board]){
        if(id_to_timeout.contains(id)){
            Time::cancel_timeout(id_to_timeout[id]);
            id_to_timeout.erase(id);
            auto it = id_to_flag.find(id);
            if (it != id_to_flag.end()) {
                *(it->second.first) = it->second.second;
            }
        }
        else{
            // InfoWarning("Reciebed order %d from board %s when it hasnt been send by the VCU", id, Board_to_ip[board].to_string().c_str());
        }
    }else{
        ErrorHandler("Order recieved from unknow ip");
    }
}

// void Ethernet::update(){
//     for(const auto& order_trigger : order_triggers){
//         if(*order_trigger.flag){
//             if(order_trigger.board == Boards::HVSCU){
//                 Socket_HVSCU.send_order(order_trigger.order);
//             }else if(order_trigger.board == Boards::BMSL){
//                 Socket_BMSL.send_order(order_trigger.order);
//             }else if(order_trigger.board == Boards::PCU){
//                 Socket_PCU.send_order(order_trigger.order);
//             }else if(order_trigger.board == Boards::LCU){
//                 Socket_LCU.send_order(order_trigger.order);
//             }else if(order_trigger.board == Boards::BLCU){
//                 Socket_BLCU.send_order(order_trigger.order);
//             }
//             *order_trigger.flag = false; // Resetear la flag
//         }
//     }
// }

bool Ethernet::connected(){
    //igual meter lo de la blcu
    return Socket_PCU.is_connected() && 
    Socket_HVSCU.is_connected() && 
    Socket_BMSL.is_connected() && 
    Socket_LCU.is_connected() && 
    Socket_BLCU.is_connected() &&
    Control_station.is_connected();
}
};