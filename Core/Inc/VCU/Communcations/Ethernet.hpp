#pragma once
#include "ST-LIB.hpp"
#include "VCU.hpp"

//Esto luego será el ethernet:
namespace Communications{
class Ethernet{
    private:

    static bool requested_unbrake;
    static bool requested_open_contactors;
    static bool requested_close_contactors;
    //Finish precharge ver como recibirlo
    static bool requested_brake;
    struct Flags_ready{
        bool requested_levitation_active;
        bool requested_propulsion_active;
        bool requested_charging_LV_battery;
        bool requested_enable_booster;
    };
    
    Flags_ready flags_ready{};
    
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
    
    StateMachine* GeneralStateMachine = nullptr;
    StateMachine* OperationalStateMachine = nullptr;
    
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
public:



inline static const IPV4 VCU_IP{"192.168.1.3"}; //Habra que ver las ips
inline static const IPV4 PCU{"192.168.1.3"};//Cambiar ips
inline static const IPV4 HVSU{"192.168.1.3"};
inline static const IPV4 BMSL{"192.168.1.3"};
inline static const IPV4 LCU{"192.168.1.3"};
inline static const IPV4 BLCU{"192.168.1.3"};
inline static const IPV4 control_station_ip{"192.168.0.9"};

inline static const MAC local_mac{"00:80:E1:11:02:00"};

inline static const uint16_t local_port{50500};//Habra que ver tambien los puertos
inline static const uint16_t pcu_port{50501};
inline static const uint16_t hvcu_port{50502};
inline static const uint16_t bmsl_port{50503};
inline static const uint16_t lcu_port{50504};
inline static const uint16_t blcu_port{50505};

inline static const uint16_t udp_port{50400};


Socket PCU;
Socket HVSU;
Socket BMSL;
Socket LCU;
Socket BLCU;

DatagramSocket packets_endpoint;
static std::vector<HeapPacket*> packets{}; //Lo que mando a la gui
Ethernet(StateMachine* GeneralStateMachine, StateMachine* OperationalStateMachine);
void initialize_state_orders();





//definir todo el ethernet


}:
}: // namespace VCU::Ethernet

