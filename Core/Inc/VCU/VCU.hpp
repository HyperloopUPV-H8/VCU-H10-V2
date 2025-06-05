#pragma once
#include "ST-LIB.hpp"
#include "VCU/Communcations/Ethernet.hpp"
#include "VCU/Actuators/Leds.hpp"
#include "VCU/Actuators/Pinout.hpp"

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
    
    Actuators::Leds leds{Pinout::led_operational_pin, Pinout::led_fault_pin, Pinout::led_can_pin, Pinout::led_sleep_pin, Pinout::led_flash_pin};
    //HeapPacket Flags{};

    StateMachine GeneralStateMachine;
    StateMachine OperationalStateMachine;

    Communications::Ethernet ethernet;


    void initialize_state_machines();


    public:
    VCU();
    void update();
    

};