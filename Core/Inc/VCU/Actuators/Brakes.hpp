#pragma once
#include "ST-LIB.hpp"

namespace Actuators {
class Brakes {
    private:
    //Faltan todo lo que es tapes
    DigitalOutput Actuator_out;

    DigitalSensor reed1_input;
    DigitalSensor reed2_input;
    DigitalSensor reed3_input;
    DigitalSensor reed4_input;
    DigitalSensor reed5_input;
    DigitalSensor reed6_input;
    DigitalSensor reed7_input;
    DigitalSensor reed8_input;

    DigitalOutput TapeOutput;
    DigitalSensor Tape_input;

    
    public:
    bool Active_brakes=true;
    bool breaks_first_time = true;

    PinState reed1= PinState::OFF;
    PinState reed2= PinState::OFF;
    PinState reed3= PinState::OFF;
    PinState reed4= PinState::OFF;
    PinState reed5= PinState::OFF;
    PinState reed6= PinState::OFF;
    PinState reed7= PinState::OFF;
    PinState reed8= PinState::OFF;

    PinState Tape_state = PinState::OFF;


    
    Brakes(Pin& actuator_pin, Pin& reed1_pin, Pin& reed2_pin, Pin& reed3_pin, Pin& reed4_pin, Pin& reed5_pin, Pin& reed6_pin, Pin& reed7_pin, Pin& reed8_pin,Pin& Tapes_input,Pin&Tapes_output)
        : Actuator_out(actuator_pin), 
          reed1_input(reed1_pin, &reed1), 
          reed2_input(reed2_pin, &reed2), 
          reed3_input(reed3_pin, &reed3), 
          reed4_input(reed4_pin, &reed4), 
          reed5_input(reed5_pin, &reed5), 
          reed6_input(reed6_pin, &reed6), 
          reed7_input(reed7_pin, &reed7), 
          reed8_input(reed8_pin, &reed8),
          Tape_input(Tapes_input, &Tape_state),
          TapeOutput(Tapes_output)
           {
        Actuator_out.turn_on();
        Tapes_output.turn_on();//Hardcodeado para desabilitar tapes creo, o al reves xd
    }

    void brake();
    void unbrake();
    void read_reeds();
}
}; 