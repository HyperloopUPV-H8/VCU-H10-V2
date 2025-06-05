#pragma once
#include "ST-LIB.hpp"

namespace Communications {

struct UDP_data {
    uint8_t prueba;
};

enum VCU_ID : uint16_t {
    VCU_Open_contactors           = 0x0001,
    VCU_Close_contactors          = 0x0002,
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

enum HVSCU_ID : uint16_t {
    Close_contactors   = 0x0001,
    Open_contactors    = 0x0002,
    Start_charging_HV  = 0x0003,
    Stop_charging_HV   = 0x0004
};

enum BMSL_ID : uint16_t {
    Start_charging_LV  = 0x0001,
    Stop_charging_LV   = 0x0002
};

enum PCU_ID : uint16_t {
    Stop_dynamic_run   = 0x0001,
    Start_dynamic_run  = 0x0002
};

enum LCU_ID : uint16_t {
    Stop_vertical_levitation    = 0x0001,
    Start_vertical_levitation   = 0x0002,
    Stop_horizontal_levitation  = 0x0003,
    Start_horizontal_levitation = 0x0004
};

enum BLCU_ID : uint16_t {
    Upload_code    = 0x0001,
    Reset_vehicle  = 0x0002
};

enum Boards {
    VCU,
    PCU,
    HVSCU,
    BMSL,
    LCU,
    BLCU
};
class Ethernet{
    private:
    
    StateMachine* GeneralStateMachine = nullptr;
    StateMachine* OperationalStateMachine = nullptr;

    //State orders:
    HeapStateOrder Open_Contactors;
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
    static void on_unbrake(){ requested_unbrake = true; }
    static void on_open_contactors(){ requested_open_contactors = true; }
    static void on_close_contactors(){ requested_close_contactors = true; }
    static void on_brake(){ requested_brake = true; }
    //Funciones de las flags, cambiar y tal a gusto:
    static void on_levitation_active(){ flags_ready.requested_levitation_active = true; }
    static void on_propulsion_active(){ flags_ready.requested_propulsion_active = true; }
    static void on_charging_LV_battery(){ flags_ready.requested_charging_LV_battery = true; }
    static void on_enable_booster(){ flags_ready.requested_enable_booster = true; }
    static void on_levitation_inactive(){ flags_ready.requested_levitation_active = false; }
    static void on_propulsion_inactive(){ flags_ready.requested_propulsion_active = false; }
    static void on_charging_LV_battery_inactive(){ flags_ready.requested_charging_LV_battery = false; }
    static void on_disable_booster(){ flags_ready.requested_enable_booster = false; }
    

    static std::vector<HeapPacket*> packets; //Lo que mando a la gui
    struct OrderTriggers{
        Boards board;
        bool* flag;
        HeapOrder* order;

    };
    static std::vector<OrderTriggers> order_triggers;

    //Hay que ver como aplico la generacion de codigo, por ahora esto:

    static bool HVSCU_requested_close_contactors;
    static bool HVSCU_requested_open_contactors;
    static bool HVSCU_requested_start_charging;
    static bool HVSCU_requested_stop_charging;

    static bool BMSL_requested_start_charging;
    static bool BMSL_requested_stop_charging;

    static bool PCU_requested_stop_dynamic_run;
    static bool PCU_requested_start_dynamic_run;

    static bool LCU_requested_stop_vertical_levitation;
    static bool LCU_requested_start_vertical_levitation;
    static bool LCU_requested_stop_horizontal_levitation;
    static bool LCU_requested_start_horizontal_levitation;

    static bool BLCU_requested_upload_code;
    static bool BLCU_requested_reset_vehicle;

    static void on_HVSCU_requested_close_contactors(){
        HVSCU_requested_close_contactors = true;
    }
    static void on_HVSCU_requested_open_contactors(){
        HVSCU_requested_open_contactors = true;
    }
    static void on_HVSCU_requested_start_charging(){
        HVSCU_requested_start_charging = true;
    }
    static void on_HVSCU_requested_stop_charging(){
        HVSCU_requested_stop_charging = true;
    }


    static void on_BMSL_requested_start_charging(){
        BMSL_requested_start_charging = true;
    }
    static void on_BMSL_requested_stop_charging(){
        BMSL_requested_stop_charging = true;
    }


    static void on_PCU_requested_stop_dynamic_run(){
        PCU_requested_stop_dynamic_run = true;
    }

    static void on_PCU_requested_start_dynamic_run(){
        PCU_requested_start_dynamic_run = true;
    }


    static void on_LCU_requested_stop_vertical_levitation(){
        LCU_requested_stop_vertical_levitation = true;
    }
    static void on_LCU_requested_start_vertical_levitation(){
        LCU_requested_start_vertical_levitation = true;
    }
    static void on_LCU_requested_stop_horizontal_levitation(){
        LCU_requested_stop_horizontal_levitation = true;
    }
    static void on_LCU_requested_start_horizontal_levitation(){
        LCU_requested_start_horizontal_levitation = true;
    }


    static void on_BLCU_requested_upload_code(){
        BLCU_requested_upload_code = true;
    }
    static void on_BLCU_requested_reset_vehicle(){
        BLCU_requested_reset_vehicle = true;
    }
    //Hay que adaptar las ordenes y tal pero por ahora las defino
    //HVSCU orders:
    HeapOrder CloseContactors{
        HVSCU_ID::Close_contactors, &HVSCU_requested_close_contactors
    };
    HeapOrder OpenContactors{
        HVSCU_ID::Open_contactors, &HVSCU_requested_open_contactors
    };
    HeapOrder StartChargingHV{
        HVSCU_ID::Start_charging_HV, &HVSCU_requested_start_charging
    };
    HeapOrder StopChargingHV{
        HVSCU_ID::Stop_charging_HV, &HVSCU_requested_stop_charging
    };
    //BMSL orders:
    HeapOrder StartChargingLV{
        BMSL_ID::Start_charging_LV, &BMSL_requested_start_charging
    };
    HeapOrder StopChargingLV{
        BMSL_ID::Stop_charging_LV, &BMSL_requested_stop_charging
    };
    //PCU orders:
    HeapOrder StopDynamicRun{
        PCU_ID::Stop_dynamic_run, &PCU_requested_stop_dynamic_run
    };
    HeapOrder StartDynamicRun{
        PCU_ID::Start_dynamic_run, &PCU_requested_start_dynamic_run
    };
    //LCU orders:
    HeapOrder StopVerticalLevitation{
        LCU_ID::Stop_vertical_levitation, &LCU_requested_stop_vertical_levitation
    };
    HeapOrder StartVerticalLevitation{
        LCU_ID::Start_vertical_levitation, &LCU_requested_start_vertical_levitation
    };
    HeapOrder StopHorizontalLevitation{
        LCU_ID::Stop_horizontal_levitation, &LCU_requested_stop_horizontal_levitation
    };
    HeapOrder StartHorizontalLevitation{
        LCU_ID::Start_horizontal_levitation, &LCU_requested_start_horizontal_levitation
    };
    //BLCU orders:
    HeapOrder UploadCode{
        BLCU_ID::Upload_code, &BLCU_requested_upload_code
    };
    HeapOrder ResetVehicle{
        BLCU_ID::Reset_vehicle, &BLCU_requested_reset_vehicle
    };

public:
    static bool requested_unbrake;
    static bool requested_open_contactors;
    static bool requested_close_contactors;
    //Finish precharge ver como recibirlo
    static bool requested_brake;
    struct Flags_ready{
        bool requested_levitation_active=false;
        bool requested_propulsion_active=false;
        bool requested_charging_LV_battery=false;
        bool requested_enable_booster=false;
    };



    inline static Flags_ready flags_ready{};


    inline static const IPV4 VCU_IP{"192.168.1.3"}; //Habra que ver las ips
    inline static const IPV4 PCU_IP{"192.168.1.3"};//Cambiar ips
    inline static const IPV4 HVSU_IP{"192.168.1.3"};
    inline static const IPV4 BMSL_IP{"192.168.1.3"};
    inline static const IPV4 LCU_IP{"192.168.1.3"};
    inline static const IPV4 BLCU_IP{"192.168.1.3"};
    inline static const IPV4 control_station_ip{"192.168.0.9"};

    inline static const MAC local_mac{"00:80:E1:11:02:00"};

    inline static const uint16_t local_port{50500};//Habra que ver tambien los puertos
    inline static const uint16_t controlstation_port{50555};
    inline static const uint16_t pcu_port{50501};
    inline static const uint16_t hvcu_port{50502};
    inline static const uint16_t bmsl_port{50503};
    inline static const uint16_t lcu_port{50504};
    inline static const uint16_t blcu_port{50505};

    inline static const uint16_t udp_controlstation_port{50400};
    inline static const uint16_t udp_port{50401};
    inline static const uint16_t pcu_udp_port{50402};
    inline static const uint16_t hvcu_udp_port{50403};
    inline static const uint16_t bmsl_udp_port{50404};
    inline static const uint16_t lcu_udp_port{50405};
    inline static const uint16_t blcu_udp_port{50406};


    Socket Socket_PCU;
    Socket Socket_HVSCU;
    Socket Socket_BMSL;
    Socket Socket_LCU;
    Socket Socket_BLCU;

    ServerSocket Control_station;

    DatagramSocket packets_endpoint;
    DatagramSocket PCU_UDP;
    DatagramSocket HVSU_UDP;
    DatagramSocket BMSL_UDP;
    DatagramSocket LCU_UDP;
    DatagramSocket BLCU_UDP;
    Ethernet(StateMachine* GeneralStateMachine, StateMachine* OperationalStateMachine);
    void update();
    void initialize_state_orders();
    bool connected();





//definir todo el ethernet


};
} // namespace Communications

