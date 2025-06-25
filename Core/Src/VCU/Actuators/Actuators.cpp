#include "VCU/Actuators/Actuators.hpp"

namespace Actuators{


    void Actuators::init(){
            Pump_c1.set_frequency(10000);
            Pump_c1.set_duty_cycle(0); 
            Pump_c1.turn_on();

            Pump_c2.set_frequency(10000);
            Pump_c2.set_duty_cycle(0); 
            Pump_c2.turn_on();

            Regulator_out1.set_frequency(30000);
            Regulator_out1.set_duty_cycle(0);
            Regulator_out1.turn_on();

            Regulator_out2.set_frequency(30000);
            Regulator_out2.set_duty_cycle(0);
            Regulator_out2.turn_on();>
    }

    void Actuators::set_regulator_1(float value){
        float regulator_value = static_cast<float>(value * (100.0 / 6));//FAlta hacer la caracterizacion pero echale que al 100 deberia sacar 6 bares
        Regulator_out1.set_duty_cycle(regulator_value);
    }

    void Actuators::set_regulator_2(float value){
        float regulator_value = static_cast<float>(value * (100.0 / 6));
        Regulator_out2.set_duty_cycle(regulator_value);
    }

    void Actuators::set_pump_1(uint32_t value){
        Pump_c1.set_duty_cycle(value);
    }
    void Actuators::set_pump_2(uint32_t value){
        Pump_c2.set_duty_cycle(value);
    }

    void Actuators::read_regulators() {
        Regulator_in1.read();
        Regulator_in2.read();
    }

    void Actuators::read_pressure(){
        PresionAlta.read();
        PresionRegulador.read();
        PresionFrenos.read();
        // PresionCapsula.read();
    }

    void Actuators::read_flow(){
        flow1_input.read();
        flow2_input.read();
    }

    void Actuators::read_sdc() {
        SDC_sensor.read();
    }
}