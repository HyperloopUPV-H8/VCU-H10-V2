#pragma once
#include "ST-LIB.hpp"
#include"Communications/Ethernet.hpp"
#include "Actuators/Leds.hpp"



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

class VCU{
    private:
    Communications::Ethernet();
    Actuators::Leds leds{Pinout::led_operational_pin, Pinout::led_fault_pin, Pinout::led_can_pin, Pinout::led_sleep_pin, Pinout::led_flash_pin};
    //declaraciones de todo :)

    static bool requested_unbrake;
    static bool requested_open_contactors;
    static bool requested_close_contactors;
    //Finish precharge ver como recibirlo
    static bool requested_brake;
    //Ordenes que estarán en state orders:
    static bool requested_levitation_active;
    static bool requested_propulsion_active;
    static bool requested_charging_LV_battery;
    static bool requested_enable_booster;

    HeapPacket Flags{};

    StateMachine GeneralStateMachine;
    StateMachine OperationalStateMachine;

    static void on_unbrake(){ requested_unbrake = true; }
    static void on_open_contactors(){ requested_open_contactors = true; }
    static void on_close_contactors(){ requested_close_contactors = true; }
    static void on_brake(){ requested_brake = true; }
    //Funciones de las flags, cambiar y tal a gusto:
    static void on_levitation_active(){ requested_levitation_active = true; }
    static void on_propulsion_active(){ requested_propulsion_active = true; }
    static void on_charging_LV_battery(){ requested_charging_LV_battery = true; }
    static void on_enable_booster(){ requested_enable_booster = true; }
    static void on_levitation_inactive(){ requested_levitation_active = false; }
    static void on_propulsion_inactive(){ requested_propulsion_active = false; }
    static void on_charging_LV_battery_inactive(){ requested_charging_LV_battery = false; }
    static void on_disable_booster(){ requested_enable_booster = false; }

    

    void initialize_state_machines();


    public:
    VCU();
    void update();
    

};