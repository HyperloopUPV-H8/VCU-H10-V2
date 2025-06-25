#pragma once
#include "ST-LIB.hpp"

namespace Actuators {

    enum Pump{
    PUMP_UNIDADES = 0,
    PUMP_PLACAS = 1
    };

    enum Regulator{
        REGULATOR_1 = 0,
        REGULATOR_2 = 1
    };
    class Actuators{
        private:

        PWM Regulator_out1;//por caracterizar
        PWM Regulator_out2;

        DigitalSensor Regulator_in1;
        DigitalSensor Regulator_in2;

        PWM Pump_c1;
        PWM Pump_c2;

        DigitalSensor flow1_input;
        DigitalSensor flow2_input;

        DigitalSensor SDC_sensor;//creo que tocará hacerlo exti

        LinearSensor <float> PresionAlta;
        LinearSensor <float> PresionRegulador;//baja
        LinearSensor <float> PresionFrenos;//baja
        LinearSensor <float> PresionCapsula;// por caracterizar


        public:
        Regulator selected_regulator;
        Pump selected_pump;
        float selected_regulator_pressure;

        PinState regulator_1_input;
        PinState regulator_2_input;  

        uint32_t selected_pump_duty;

        PinState flow1;
        PinState flow2; 

        PinState Sdc;

        float pressure_1 = 0;
        float pressure_2 = 0;
        float pressure_3 = 0;
        float pressure_4 = 0;

        Actuators(Pin& regulator_1_in_pin, Pin& regulator_2_in_pin, Pin& regulator_1_out_pin, Pin& regulator_2_out_pin,
                  Pin& pump_c1_pin, Pin& pump_c2_pin, Pin& flow1_pin, Pin& flow2_pin,Pin& sdc_sensor_pin,
                  Pin& pressure_alta_pin, Pin& pressure_regulador_pin, Pin& pressure_frenos_pin, Pin& pressure_capsula_pin) :
            Regulator_out1(regulator_1_out_pin),
            Regulator_out2(regulator_2_out_pin),
            Regulator_in1(regulator_1_in_pin,&regulator_1_input),
            Regulator_in2(regulator_2_in_pin,&regulator_2_input),
            Pump_c1(pump_c1_pin),
            Pump_c2(pump_c2_pin),
            flow1_input(flow1_pin,&flow1),
            flow2_input(flow2_pin,&flow2),
            SDC_sensor(sdc_sensor_pin,&Sdc),
            PresionAlta(pressure_alta_pin,0.00763,0.318,&pressure_1),
            PresionRegulador(pressure_regulador_pin,0.256,0.375,&pressure_2),
            PresionFrenos(pressure_frenos_pin,0.256,0.375,&pressure_3),
            PresionCapsula(pressure_capsula_pin,1,0,&pressure_4)//Falta por caracterizar
             {

             }

        void init();
        void set_regulator_1(float value);
        void set_regulator_2(float value);
        void set_pump_1(uint32_t value);
        void set_pump_2(uint32_t value);
        void read_regulators();
        void read_pressure();
        void read_flow();
        void read_sdc();


    };
}