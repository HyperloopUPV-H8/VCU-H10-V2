#include "VCU/Brakes.hpp"
#include "VCU/Pinout.hpp"

Brakes::Brakes(){
    Actuator_out = DigitalOutput(BRAKE_PIN);
    
    reed1_input = DigitalSensor(REED1_PIN, &reed1);
    reed2_input = DigitalSensor(REED2_PIN, &reed2);
    reed3_input = DigitalSensor(REED3_PIN, &reed3);
    reed4_input = DigitalSensor(REED4_PIN, &reed4);
    reed5_input = DigitalSensor(REED5_PIN, &reed5);
    reed6_input = DigitalSensor(REED6_PIN, &reed6);
    reed7_input = DigitalSensor(REED7_PIN, &reed7);
    reed8_input = DigitalSensor(REED8_PIN, &reed8);

    // Tape_input = DigitalSensor(TAPE_INPUT_PIN, &Tape_state);
    tape_enable_output = DigitalOutput(TAPE_E_PIN);

    tape_emergency_input = DigitalSensor(TAPE_PIN, &tape_emergency);
}

void Brakes::init(){
    Actuator_out.turn_on();
    tape_enable_output.turn_on();//Hardcodeado para desabilitar tapes creo, o al reves xd
}

void Brakes::brake(){
    if(breaks_first_time){
        breaks_first_time = false;

    }
    // TO DO: SET UP REGULATOR PRESSURE TO 6 bars

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
    reed5_input.read();
    reed6_input.read();
    reed7_input.read();
    reed8_input.read();

    All_reeds = reed1 && 
                reed2 &&
                reed3 &&
                reed4 &&
                reed5 &&
                reed6 &&
                reed7 &&
                reed8;
}

void Brakes::read_tape_emergency() {
    tape_emergency_input.read();
}
