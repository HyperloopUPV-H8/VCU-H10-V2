#pragma once
#include "ST-LIB.hpp"

namespace Actuators {
    class Actuators{
        private:

        PWM Regulator_out1;//por caracterizar
        PWM Regulator_out2;

        DigitalSensor Regulator_in1;
        DigitalSensor Regulator_in2;

        PWM Pump_c1;
        PWM Pump_c2;

        DigitalSensor SDC_sensor;

        LinearSensor <float> PresionAlta;
        LinearSensor <float> PresionRegulador;//baja
        LinearSensor <float> PresionFrenos;//baja
        LinearSensor <float> PresionCapsula;// por caracterizar


        public:
        float selected_regulator_pressure1;
        float selected_regulator_pressure2;

        PinState regulator_1_input;
        PinState regulator_2_input;  

        uint32_t pump1;
        uint32_t pump2;

        PinState flow1;
        PinState flow2; 

        PinState Sdc;

        float pressure_1 = 0;
        float pressure_2 = 0;
        float pressure_3 = 0;
        float pressure_4 = 0;

        // Actuators()

    }
};