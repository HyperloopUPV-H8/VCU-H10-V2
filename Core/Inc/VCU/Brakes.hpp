#pragma once
#include "ST-LIB.hpp"

class Brakes {
   private:
    // Faltan todo lo que es tapes
    DigitalOutput Actuator_out;

    DigitalSensor reed1_input;
    DigitalSensor reed2_input;
    DigitalSensor reed3_input;
    DigitalSensor reed4_input;

    // DigitalSensor Tape_input;

    DigitalSensor tape_emergency_input;

   public:
    bool Active_brakes = true;
    bool breaks_first_time = true;

    PinState reed1 = PinState::OFF;
    PinState reed2 = PinState::OFF;
    PinState reed3 = PinState::OFF;
    PinState reed4 = PinState::OFF;

    bool All_reeds = false;

    PinState tape_emergency = PinState::ON;
    DigitalOutput tape_enable_output;

    PinState tape_enable_status = PinState::OFF;
    
    Brakes();

    void init();
    void brake();
    void unbrake();
    void read_reeds();
    void read_tape_emergency();
};