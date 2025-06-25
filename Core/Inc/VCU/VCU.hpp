#pragma once
#include "ST-LIB.hpp"
#include "VCU/Communcations/Ethernet.hpp"
#include "VCU/Actuators/Leds.hpp"
#include "VCU/Actuators/Pinout.hpp"
#include "VCU/Actuators/Actuators.hpp"
#include "VCU/Actuators/Brakes.hpp"

enum GeneralStates {
    Connecting,
    Operational,
    Fault,
};

enum OperationalStates {    
    Idle,
    EndOfRun,
    Energyzed,
    Ready,
};

class VCU{
    private:
    
    Actuators::Leds leds;
    Actuators::Actuators Actuators;
    Actuators::Brakes Brakes;

    static StateMachine GeneralStateMachine;
    static StateMachine OperationalStateMachine;

    Communications::Ethernet ethernet;


    void initialize_state_machines();


    public:
    VCU();
    void update();
    

};