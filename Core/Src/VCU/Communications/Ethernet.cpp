#include "VCU/Communcations/Ethernet.hpp"

namespace Communications {

    StateMachine* Ethernet::GeneralStateMachine = nullptr;
    StateMachine* Ethernet::OperationalStateMachine = nullptr;
    Actuators::Actuators* Ethernet::Actuators = nullptr;
    Actuators::Brakes* Ethernet::Brakes = nullptr;
    HeapOrder* Ethernet::Potencia_refri = nullptr;
    HeapOrder* Ethernet::Set_Regulator = nullptr;

    HeapStateOrder* Ethernet::Open_Contactors = nullptr;
    HeapStateOrder* Ethernet::Close_Contactors = nullptr;
    HeapStateOrder* Ethernet::Unbrake = nullptr;
    HeapStateOrder* Ethernet::Brake = nullptr;
    HeapStateOrder* Ethernet::EndOfRun = nullptr;
    HeapStateOrder* Ethernet::Levitation_Active = nullptr;
    HeapStateOrder* Ethernet::Propulsion_Active = nullptr;
    HeapStateOrder* Ethernet::Charging_LV_Battery = nullptr;
    HeapStateOrder* Ethernet::Enable_Booster = nullptr;
    HeapStateOrder* Ethernet::Levitation_Inactive = nullptr;
    HeapStateOrder* Ethernet::Propulsion_Inactive = nullptr;
    HeapStateOrder* Ethernet::Charging_LV_Battery_Inactive = nullptr;
    HeapStateOrder* Ethernet::Disable_booster = nullptr;

    Flags_ready Ethernet::flags_ready{};
    

Ethernet::Ethernet(StateMachine* GeneralStateMachine, StateMachine* OperationalStateMachine,Actuators::Actuators* actuators, Actuators::Brakes* brakes) {
    this->GeneralStateMachine = GeneralStateMachine;
    this->OperationalStateMachine = OperationalStateMachine;
    this->Actuators = actuators;
    this->Brakes = brakes;
    
    Control_station=ServerSocket(VCU_IP,local_port);
    //LO comento para probar sin tener todo conectado
    // Socket_PCU = Socket(VCU_IP,pcu_port,PCU_IP,local_port);
    // Socket_HVSCU = Socket(VCU_IP,hvscu_port,HVSCU_IP,local_port);
    // Socket_BMSL = Socket(VCU_IP,bmsl_port,BMSL_IP,local_port);
    // Socket_LCU = Socket(VCU_IP,lcu_port,LCU_IP,local_port);
    // Socket_BLCU = Socket(VCU_IP,blcu_port,BLCU_IP,local_port);

    packets_endpoint =DatagramSocket(VCU_IP, udp_controlstation_port,control_station_ip, udp_port);

    Socket_to_board[Boards::PCU] = &Socket_PCU;
    Socket_to_board[Boards::HVSCU] = &Socket_HVSCU;
    Socket_to_board[Boards::BMSL] = &Socket_BMSL;
    Socket_to_board[Boards::LCU] = &Socket_LCU;
    Socket_to_board[Boards::BLCU] = &Socket_BLCU;
    //falta la bcu

    Potencia_refri = new HeapOrder(Orders_id::Potencia_refri_id, &on_potencia_refri, &this->Actuators->selected_pump_duty, &this->Actuators->selected_pump);  
    Set_Regulator = new HeapOrder(Orders_id::Set_regulator_id, &on_Set_regulator, &this->Actuators->selected_regulator_pressure, &this->Actuators->selected_regulator);
    Enable_tapes = new HeapOrder(Orders_id::Enable_tapes_id, &on_Enable_tapes);
    Disable_tapes = new HeapOrder(Orders_id::Disable_tapes_id, &on_Disable_tapes);

    Reeds = new HeapPacket(Packets_id::Reeds, &brakes->reed1, &brakes->reed2, &brakes->reed3, &brakes->reed4, &brakes->reed5, &brakes->reed6, &brakes->reed7, &brakes->reed8,&brakes->All_reeds);
    flow = new HeapPacket(Packets_id::Flow, &actuators->flow1, &actuators->flow2);
    Regulator = new HeapPacket(Packets_id::Regulator, &actuators->regulator_1_input, &actuators->regulator_2_input);
    Pressure = new HeapPacket(Packets_id::Pressure, &actuators->pressure_1, &actuators->pressure_2, &actuators->pressure_3, &actuators->pressure_4);
    // Tapes = new HeapPacket(1354, &brakes->Tape_state);
    Tapes_enable = new HeapPacket(Packets_id::Tapes_enable, &brakes->Tape_enabled);

    Time::register_low_precision_alarm(16, [&]() {
        packets_endpoint.send_packet(*Reeds);
        packets_endpoint.send_packet(*flow);
        packets_endpoint.send_packet(*Regulator);
        packets_endpoint.send_packet(*Pressure);
        // packets_endpoint.send_packet(*Tapes);

    });
}

void Ethernet::recieve_order(Boards board, HeapStateOrder* Order,Orders_id id){//No hace falta mandar la placa 
    if(Order->remote_ip==nullptr){
        ErrorHandler("Order ip is null");
        return;
    }
    string remote_ip =*(Order->remote_ip);
    if(remote_ip==control_station_ip){//en pruebas esto!!
        if(id_to_pending.contains(id)){
            id_to_pending[id].control_station= true;
            return;
        }
    }if(remote_ip==Board_to_ip[board]){
        if(id_to_pending.contains(id)){
            id_to_pending[id].board= true;
            return;
        }
    }else{
        ErrorHandler("Order recieved from unknow ip");
    }
}

void Ethernet::update(){
    for(auto& pending : id_to_pending){
        if(pending.second.control_station){
                Socket_to_board[id_to_orders[pending.first].Board]->send_order(*id_to_orders[pending.first].order);
                id_to_timeout[pending.first]=Time::set_timeout(5000,[&](){
                    //InfoWarning::InfoWarningTrigger("Timeout for order to control station");
                    if(id_to_timeout.contains(pending.first)){
                        Time::cancel_timeout(id_to_timeout[pending.first]);//No se si esto es valido
                        id_to_timeout.erase(pending.first);
                    }
                });
            
        }
        else if(pending.second.board){
            if(id_to_timeout.contains(pending.first)){
                Time::cancel_timeout(id_to_timeout[pending.first]);
                id_to_timeout.erase(pending.first);
                auto it = id_to_flag.find(pending.first);
                if (it != id_to_flag.end()) {
                    *(it->second.first) = it->second.second;
                }
            }
        }
        }
}


bool Ethernet::connected(){
    //igual meter lo de la blcu
    return Socket_PCU.is_connected() && 
    Socket_HVSCU.is_connected() && 
    Socket_BMSL.is_connected() && 
    Socket_LCU.is_connected() && 
    Socket_BLCU.is_connected() &&
    Control_station.is_connected();
}

void Ethernet::initialize_state_orders() {
    Open_Contactors = new HeapStateOrder(Orders_id::Open_contactors, &Ethernet::on_open_contactors, *GeneralStateMachine, GeneralStates::Operational);
    Close_Contactors = new HeapStateOrder(Orders_id::Close_contactors, &Ethernet::on_close_contactors, *GeneralStateMachine, GeneralStates::Operational);
    Unbrake = new HeapStateOrder(Orders_id::Unbrake, &Ethernet::on_unbrake, *GeneralStateMachine, GeneralStates::Operational);
    Brake = new HeapStateOrder(Orders_id::Brake, &Ethernet::on_brake, *GeneralStateMachine, GeneralStates::Operational);
    EndOfRun = new HeapStateOrder(Orders_id::EndOfRun_id, &Ethernet::on_brake, *OperationalStateMachine, OperationalStates::EndOfRun);

    Levitation_Active = new HeapStateOrder(Orders_id::Levitation_active, &Ethernet::on_levitation_active, *OperationalStateMachine, OperationalStates::Ready);
    Propulsion_Active = new HeapStateOrder(Orders_id::Propulsion_active, &Ethernet::on_propulsion_active, *OperationalStateMachine, OperationalStates::Ready);
    Charging_LV_Battery = new HeapStateOrder(Orders_id::Charging_LV_battery, &Ethernet::on_charging_LV_battery, *OperationalStateMachine, OperationalStates::Ready);
    Enable_Booster = new HeapStateOrder(Orders_id::Enable_booster, &Ethernet::on_enable_booster, *OperationalStateMachine, OperationalStates::Ready);
    Levitation_Inactive = new HeapStateOrder(Orders_id::Levitation_inactive, &Ethernet::on_levitation_inactive, *OperationalStateMachine, OperationalStates::Ready);
    Propulsion_Inactive = new HeapStateOrder(Orders_id::Propulsion_inactive, &Ethernet::on_propulsion_inactive, *OperationalStateMachine, OperationalStates::Ready);
    Charging_LV_Battery_Inactive = new HeapStateOrder(Orders_id::Charging_LV_battery_inactive, &Ethernet::on_charging_LV_battery_inactive, *OperationalStateMachine, OperationalStates::Ready);
    Disable_booster = new HeapStateOrder(Orders_id::Disable_booster, &Ethernet::on_disable_booster, *OperationalStateMachine, OperationalStates::Ready);
}
}