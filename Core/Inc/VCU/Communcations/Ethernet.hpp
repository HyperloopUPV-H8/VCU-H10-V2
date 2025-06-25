#pragma once
#include "ST-LIB.hpp"
#include "VCU/Actuators/Actuators.hpp"
#include "VCU/Actuators/Brakes.hpp"

namespace Communications {

enum GeneralStates {
    Connecting,
    Operational,
    Fault,
};

enum OperationalStates {    
    Idle,
    Precharge,
    EndOfRun,
    Energyzed,
    Ready,
};

struct Flags_udp {
    uint8_t prueba;//Aqui van las flags
};
//IDs inventadas excepto las de id con 200
enum Orders_id : uint16_t {
    Open_contactors               = 1000,
    Close_contactors              = 1001,
    Unbrake                       =201,
    Brake                         =200,
    Potencia_refri_id             =202,
    Set_regulator_id              =203,
    Enable_tapes_id               =204,
    Disable_tapes_id              =205,
    EndOfRun_id                   = 1002,
    Levitation_active             = 1003,
    Propulsion_active             = 1004,
    Charging_LV_battery           = 1005,
    Enable_booster                = 1006,
    Levitation_inactive           = 1007,
    Propulsion_inactive           = 1008,
    Charging_LV_battery_inactive  = 1009,
    Disable_booster               = 1010
};

enum Packets_id: uint16_t{
    Flow                          =250,
    Reeds                         =251,
    Regulator                     =252,
    Pressure                      =253,
    Tapes_input                   =254,
    Tapes_enable                  =255
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

struct PendingOrder {
    bool control_station;
    bool board;
};

struct BoardOrder {
    Boards Board;
    HeapStateOrder* order;
};

class Ethernet{

    public:
    static Flags_ready flags_ready;
    inline static bool requested_open_contactors = false;
    inline static bool requested_close_contactors=false;
    inline static bool requested_end_of_run=false;

    

    inline static const string VCU_IP{"192.168.1.3"}; //Habra que ver las ips
    inline static const string PCU_IP{"192.168.1.4"};
    inline static const string HVSCU_IP{"192.168.1.5"};
    inline static const string BMSL_IP{"192.168.1.6"};
    inline static const string LCU_IP{"192.168.1.7"};
    inline static const string BLCU_IP{"192.168.1.8"};
    inline static const string control_station_ip{"192.168.0.9"};

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



    Ethernet(StateMachine* GeneralStateMachine, StateMachine* OperationalStateMachine,Actuators::Actuators* actuators, Actuators::Brakes* brakes);
    void update();
    bool connected();
    void initialize_state_orders();

    private:

    static void recieve_order(Boards board, HeapStateOrder* Order,Orders_id id);
    
    static StateMachine* GeneralStateMachine;
    static StateMachine* OperationalStateMachine;
    static Actuators::Actuators* Actuators;
    static Actuators::Brakes* Brakes;

    //Heap Orders:
    static HeapOrder* Potencia_refri;
    static HeapOrder* Set_Regulator;
    static HeapOrder* Enable_tapes;
    static HeapOrder* Disable_tapes;

    //State orders: 
    static HeapStateOrder* Open_Contactors;
    static HeapStateOrder* Close_Contactors;
    static HeapStateOrder* Unbrake;
    static HeapStateOrder* Brake;
    static HeapStateOrder* EndOfRun;

    static HeapStateOrder* Levitation_Active;
    static HeapStateOrder* Propulsion_Active;
    static HeapStateOrder* Charging_LV_Battery;
    static HeapStateOrder* Enable_Booster;
    static HeapStateOrder* Levitation_Inactive;
    static HeapStateOrder* Propulsion_Inactive;
    static HeapStateOrder* Charging_LV_Battery_Inactive;
    static HeapStateOrder* Disable_booster;

    
    static void on_brake(){
        Brakes->brake();
    }
    static void on_unbrake(){
        Brakes->unbrake();
    }
    static void on_end_of_run(){
        requested_end_of_run = true;
    }

    static void on_potencia_refri(){
        if(Actuators->selected_pump == Actuators::Pump::PUMP_UNIDADES) {
            Actuators->set_pump_1(Actuators->selected_pump_duty);
        } else if (Actuators->selected_pump == Actuators::Pump::PUMP_PLACAS) {
            Actuators->set_pump_2(Actuators->selected_pump_duty);
        }
    }

    static void on_Set_regulator(){
        if(Actuators->selected_regulator == Actuators::Regulator::REGULATOR_1){
            Actuators->set_regulator_1(Actuators->selected_regulator_pressure);
        }else if(Actuators->selected_regulator == Actuators::Regulator::REGULATOR_2){
            Actuators->set_regulator_2(Actuators->selected_regulator_pressure);
        }
    }

    static void on_Enable_tapes(){
        Brakes->Tape_enabled = true;
        Brakes->Tape_output.turn_on();
    }

    static void on_Disable_tapes(){
        Brakes->Tape_enabled = false;
        Brakes->Tape_output.turn_off();
    }
    //Funciones de las flags, y las que se mandan a otras placas, cambiar y tal al gusto:
    static void on_open_contactors(){recieve_order(Boards::HVSCU, Open_Contactors, Orders_id::Open_contactors);}
    static void on_close_contactors(){recieve_order(Boards::HVSCU, Close_Contactors, Orders_id::Close_contactors);}
    static void on_levitation_active() { recieve_order(Boards::LCU, Levitation_Active, Orders_id::Levitation_active); }//No hace falta mandar la placa
    static void on_propulsion_active() { recieve_order(Boards::PCU, Propulsion_Active, Orders_id::Propulsion_active); }
    static void on_charging_LV_battery() { recieve_order(Boards::BMSL, Charging_LV_Battery, Orders_id::Charging_LV_battery); }
    static void on_enable_booster() { recieve_order(Boards::BCU, Enable_Booster, Orders_id::Enable_booster); }
    static void on_levitation_inactive() { recieve_order(Boards::LCU, Levitation_Inactive, Orders_id::Levitation_inactive); }
    static void on_propulsion_inactive() { recieve_order(Boards::PCU, Propulsion_Inactive, Orders_id::Propulsion_inactive); }
    static void on_charging_LV_battery_inactive() { recieve_order(Boards::BMSL, Charging_LV_Battery_Inactive, Orders_id::Charging_LV_battery_inactive); }
    static void on_disable_booster() { recieve_order(Boards::BCU, Disable_booster, Orders_id::Disable_booster); }
    

    inline static std::unordered_map<Boards, Socket*> Socket_to_board{};
    inline static std::unordered_map<Boards, string> Board_to_ip{
        {Boards::PCU, PCU_IP},
        {Boards::HVSCU, HVSCU_IP},
        {Boards::BMSL, BMSL_IP},
        {Boards::LCU, LCU_IP},
        {Boards::BLCU, BLCU_IP}
    };
    inline static std::unordered_map<Orders_id, PendingOrder> id_to_pending{
        {Orders_id::Levitation_active, {false, false}},
        {Orders_id::Propulsion_active, {false, false}},
        {Orders_id::Charging_LV_battery, {false, false}},
        {Orders_id::Enable_booster, {false, false}},
        {Orders_id::Open_contactors, {false, false}},
        // {Orders_id::Brake, {false, false}},
        {Orders_id::Close_contactors, {false, false}},
        // {Orders_id::Unbrake, {false, false}},
        {Orders_id::Levitation_inactive, {false, false}},
        {Orders_id::Propulsion_inactive, {false, false}},
        {Orders_id::Charging_LV_battery_inactive, {false, false}},
        {Orders_id::Disable_booster, {false, false}}
    };
    inline static std::unordered_map<Orders_id, std::pair<bool*, bool>> id_to_flag{
        {Orders_id::Levitation_active, {&flags_ready.requested_levitation_active, true}},
        {Orders_id::Propulsion_active, {&flags_ready.requested_propulsion_active, true}},
        {Orders_id::Charging_LV_battery, {&flags_ready.requested_charging_LV_battery, true}},
        {Orders_id::Enable_booster, {&flags_ready.requested_enable_booster, true}},
        {Orders_id::Open_contactors, {&requested_open_contactors, true}},
        // {Orders_id::Brake, {&requested_brake, true}},
        {Orders_id::Levitation_inactive, {&flags_ready.requested_levitation_active, false}},
        {Orders_id::Propulsion_inactive, {&flags_ready.requested_propulsion_active, false}},
        {Orders_id::Charging_LV_battery_inactive, {&flags_ready.requested_charging_LV_battery, false}},
        {Orders_id::Disable_booster, {&flags_ready.requested_enable_booster, false}},
        {Orders_id::Close_contactors, {&requested_close_contactors, true}},
        // {Orders_id::Unbrake, {&requested_unbrake, true}}
        // {Orders_id::EndOfRun_id, {&requested_end_of_run, true}}
    };
    inline static std::unordered_map<Orders_id, BoardOrder> id_to_orders{
        {Orders_id::Levitation_active, {Boards::LCU, Levitation_Active}},
        {Orders_id::Propulsion_active, {Boards::PCU, Propulsion_Active}},
        {Orders_id::Charging_LV_battery, {Boards::BMSL, Charging_LV_Battery}},
        {Orders_id::Enable_booster, {Boards::BCU, Enable_Booster}},
        {Orders_id::Open_contactors, {Boards::HVSCU, Open_Contactors}},
        // {Orders_id::Brake, {Boards::, Brake}},
        {Orders_id::Close_contactors, {Boards::HVSCU, Close_Contactors}},
        // {Orders_id::Unbrake, {Boards::PCU, Unbrake}},
        {Orders_id::Levitation_inactive, {Boards::LCU, Levitation_Inactive}},
        {Orders_id::Propulsion_inactive, {Boards::PCU, Propulsion_Inactive}},
        {Orders_id::Charging_LV_battery_inactive, {Boards::BMSL, Charging_LV_Battery_Inactive}},
        {Orders_id::Disable_booster, {Boards::BCU, Disable_booster}}
    };
    inline static std::unordered_map<Orders_id, uint8_t> id_to_timeout{};

    HeapPacket* Reeds = nullptr;
    HeapPacket* flow = nullptr;
    HeapPacket* Regulator = nullptr;
    HeapPacket* Pressure = nullptr;
    // HeapPacket* Tapes = nullptr;
    HeapPacket* Tapes_enable = nullptr;

    // static std::vector<HeapPacket*> packets{}; //Lo que mando a la gui
    // struct OrderTriggers{
    //     Boards board;
    //     bool* flag;
    //     HeapOrder* order;

    // };
    // static std::vector<OrderTriggers> order_triggers;


};
} // namespace Communications

