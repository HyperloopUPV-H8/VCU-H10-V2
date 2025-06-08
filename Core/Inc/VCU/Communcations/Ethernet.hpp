#pragma once
#include "ST-LIB.hpp"

namespace Communications {



struct UDP_data {
    uint8_t prueba;//Aqui van las flags
};

enum Orders_id : uint16_t {
    Open_contactors           = 0x0001,
    Close_contactors          = 0x0002,
    Unbrake                       = 0x0003,
    Brake                         = 0x0004,
    Levitation_active             = 0x0005,
    Propulsion_active             = 0x0006,
    Charging_LV_battery           = 0x0007,
    Enable_booster                = 0x0008,
    Levitation_inactive           = 0x0009,
    Propulsion_inactive           = 0x0010,
    Charging_LV_battery_inactive  = 0x0011,
    Disable_booster               = 0x0012
};

enum class Boards : uint8_t {
    PCU,
    HVSCU,
    BMSL,
    LCU,
    BLCU,
    BCU
};

struct Flags_ready{
    bool requested_levitation_active = false;
    bool requested_propulsion_active = false;
    bool requested_charging_LV_battery = false;
    bool requested_enable_booster = false;
};

class Ethernet{
    private:

    static void send_order(Boards board, HeapStateOrder* Order,Orders_id id);
    
    StateMachine* GeneralStateMachine = nullptr;
    StateMachine* OperationalStateMachine = nullptr;

    //State orders: Que probablemente mande a tomar por culo
    HeapStateOrder* Open_Contactors;
    HeapStateOrder Close_Contactors;
    HeapStateOrder Unbrake;
    HeapStateOrder Brake;
    
    HeapStateOrder Levitation_Active;
    HeapStateOrder Propulsion_Active;
    HeapStateOrder Charging_LV_Battery;
    HeapStateOrder Enable_Booster;
    HeapStateOrder Levitation_Inactive;
    HeapStateOrder Propulsion_Inactive;
    HeapStateOrder Charging_LV_Battery_Inactive;
    HeapStateOrder Disable_booster;

    static void on_open_contactors(){
        requested_open_contactors = true; 
    }
    static void on_close_contactors(){
        requested_open_contactors = false; 
    }
    static void on_brake(){
        requested_brake = true; 
    }
    static void on_unbrake(){
        requested_brake=false;
    }
    //Funciones de las flags, cambiar y tal al gusto:
    void on_levitation_active(){ 
        send_order(Boards::LCU,&Levitation_Active,Orders_id::Levitation_active); 
    }
    void on_propulsion_active(){
        send_order(Boards::PCU,&Propulsion_Active,Orders_id::Propulsion_active);
    }
    void on_charging_LV_battery(){
        send_order(Boards::BMSL,&Charging_LV_Battery,Orders_id::Charging_LV_battery);
    }
    void on_enable_booster(){ 
        send_order(Boards::BCU,&Enable_Booster,Orders_id::Enable_booster);
     }
    void on_levitation_inactive(){
        send_order(Boards::LCU, &Levitation_Inactive, Orders_id::Levitation_inactive);
    }
    void on_propulsion_inactive(){
        send_order(Boards::PCU, &Propulsion_Inactive, Orders_id::Propulsion_inactive);
    }
    void on_charging_LV_battery_inactive(){
        send_order(Boards::BMSL, &Charging_LV_Battery_Inactive, Orders_id::Charging_LV_battery_inactive);
    }
    void on_disable_booster(){
        send_order(Boards::BCU, &Disable_booster, Orders_id::Disable_booster);
    }
    

    // static std::vector<HeapPacket*> packets{}; //Lo que mando a la gui
    // struct OrderTriggers{
    //     Boards board;
    //     bool* flag;
    //     HeapOrder* order;

    // };
    // static std::vector<OrderTriggers> order_triggers;

    //Hay que ver como aplico la generacion de codigo, por ahora esto:

    

public:
    static Flags_ready flags_ready;
    inline static bool requested_open_contactors = false;
    inline static bool requested_brake = false;

    

    inline static const IPV4 VCU_IP{"192.168.1.3"}; //Habra que ver las ips
    inline static const IPV4 PCU_IP{"192.168.1.3"};//Cambiar ips
    inline static const IPV4 HVSCU_IP{"192.168.1.3"};
    inline static const IPV4 BMSL_IP{"192.168.1.3"};
    inline static const IPV4 LCU_IP{"192.168.1.3"};
    inline static const IPV4 BLCU_IP{"192.168.1.3"};
    inline static const IPV4 control_station_ip{"192.168.0.9"};

    inline static const MAC local_mac{"00:80:E1:11:02:00"};

    inline static const uint16_t local_port{50500};//Habra que ver tambien los puertos
    inline static const uint16_t controlstation_port{50555};
    inline static const uint16_t pcu_port{50501};
    inline static const uint16_t hvscu_port{50502};
    inline static const uint16_t bmsl_port{50503};
    inline static const uint16_t lcu_port{50504};
    inline static const uint16_t blcu_port{50505};

    inline static const uint16_t udp_controlstation_port{50400};
    inline static const uint16_t udp_port{50401};
    // inline static const uint16_t pcu_udp_port{50402};
    // inline static const uint16_t hvcu_udp_port{50403};
    // inline static const uint16_t bmsl_udp_port{50404};
    // inline static const uint16_t lcu_udp_port{50405};
    // inline static const uint16_t blcu_udp_port{50406};


    Socket Socket_PCU;
    Socket Socket_HVSCU;
    Socket Socket_BMSL;
    Socket Socket_LCU;
    Socket Socket_BLCU;

    ServerSocket Control_station;

    DatagramSocket packets_endpoint;
    // DatagramSocket PCU_UDP;
    // DatagramSocket HVSU_UDP;
    // DatagramSocket BMSL_UDP;
    // DatagramSocket LCU_UDP;
    // DatagramSocket BLCU_UDP;

    inline static std::unordered_map<Boards, Socket*> Socket_to_board{};
    inline static std::unordered_map<Boards, IPV4> Board_to_ip{
        {Boards::PCU, PCU_IP},
        {Boards::HVSCU, HVSCU_IP},
        {Boards::BMSL, BMSL_IP},
        {Boards::LCU, LCU_IP},
        {Boards::BLCU, BLCU_IP}
    };

    inline static std::unordered_map<Orders_id, std::pair<bool*, bool>> id_to_flag{
        {Orders_id::Levitation_active, {&flags_ready.requested_levitation_active, true}},
        {Orders_id::Propulsion_active, {&flags_ready.requested_propulsion_active, true}},
        {Orders_id::Charging_LV_battery, {&flags_ready.requested_charging_LV_battery, true}},
        {Orders_id::Enable_booster, {&flags_ready.requested_enable_booster, true}},
        {Orders_id::Open_contactors, {&requested_open_contactors, true}},
        {Orders_id::Brake, {&requested_brake, true}},
        {Orders_id::Levitation_inactive, {&flags_ready.requested_levitation_active, false}},
        {Orders_id::Propulsion_inactive, {&flags_ready.requested_propulsion_active, false}},
        {Orders_id::Charging_LV_battery_inactive, {&flags_ready.requested_charging_LV_battery, false}},
        {Orders_id::Disable_booster, {&flags_ready.requested_enable_booster, false}},
        {Orders_id::Close_contactors, {&requested_open_contactors, false}},
        {Orders_id::Unbrake, {&requested_brake, false}}
    };



    Ethernet(StateMachine* GeneralStateMachine, StateMachine* OperationalStateMachine);
    // void update();
    bool connected();





//definir todo el ethernet


};
} // namespace Communications

