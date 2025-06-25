#include "VCU/Actuators/Brakes.hpp"

namespace Actuators {

    void Brakes::init(){
        Actuator_out.turn_on();
        Tape_output.turn_on();//Hardcodeado para desabilitar tapes creo, o al reves xd
    }

    void Brakes::brake(){
        if(breaks_first_time){
            breaks_first_time = false;

        }
        Actuator_out.turn_on();
        Active_brakes = true;
    }

    void Brakes::unbrake(){
        Actuator_out.turn_off();
        Active_brakes = false;
    }

    void Brakes::read_reeds() {
        reed1_input.read();
        reed2_input.read();
        reed3_input.read();
        reed4_input.read();
        reed5_input.read();
        reed6_input.read();
        reed7_input.read();
        reed8_input.read();
    }
}