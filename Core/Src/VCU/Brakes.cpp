#include "VCU/Brakes.hpp"
#include "VCU/Pinout.hpp"

Brakes::Brakes(){
    Actuator_out = DigitalOutput(BRAKE_PIN);
    
    reed1_input = DigitalSensor(REED1_PIN, &reed1);
    reed2_input = DigitalSensor(REED2_PIN, &reed2);
    reed3_input = DigitalSensor(REED3_PIN, &reed3);
    reed4_input = DigitalSensor(REED4_PIN, &reed4);

    tape_enable_output = DigitalOutput(TAPE_E_PIN);

    tape_emergency_input = DigitalSensor(TAPE_PIN, &tape_emergency);
}

void Brakes::init(){
    Actuator_out.turn_off();
    tape_enable_output.turn_on();
}

void Brakes::brake(){
    if(breaks_first_time){
        breaks_first_time = false;

    }

    Actuator_out.turn_off();
    Active_brakes = true;
}

void Brakes::unbrake(){
    Actuator_out.turn_on();
    Active_brakes = false;
}

void Brakes::read_reeds() {
    reed1_input.read();
    reed2_input.read();
    reed3_input.read();
    reed4_input.read();

    All_reeds = reed1 && 
                reed2 &&
                reed3 &&
                reed4;
}

void Brakes::read_tape_emergency() {
    tape_emergency_input.read();
}
