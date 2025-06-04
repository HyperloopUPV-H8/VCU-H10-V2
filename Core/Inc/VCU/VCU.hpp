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
    
    Actuators::Leds leds{Pinout::led_operational_pin, Pinout::led_fault_pin, Pinout::led_can_pin, Pinout::led_sleep_pin, Pinout::led_flash_pin};
    //declaraciones de todo :)

    

    HeapPacket Flags{};

    StateMachine GeneralStateMachine;
    StateMachine OperationalStateMachine;

    Communications::Ethernet(&GeneralStateMachine,&OperationalStateMachine);


    

    void initialize_state_machines();


    public:
    VCU();
    void update();
    

};