#pragma once
#include "ST-LIB.hpp"

class Actuators {
   private:
    PWM Regulator_out1;  // por caracterizar
    PWM Regulator_out2;

    DigitalSensor Regulator_in1;
    DigitalSensor Regulator_in2;

    PWM Pump_c1;
    PWM Pump_c2;

    DigitalSensor flow1_input;
    DigitalSensor flow2_input;

    DigitalSensor SDC_sensor;  // creo que tocará hacerlo exti

    LinearSensor<float> PresionAlta;
    LinearSensor<float> PresionRegulador;  // baja
    LinearSensor<float> PresionFrenos;     // baja
    LinearSensor<float> PresionCapsula;    // por caracterizar

   public:
    enum Pump { PUMP_UNIDADES = 0, PUMP_PLACAS = 1 };

    enum Regulator { REGULATOR_1 = 0, REGULATOR_2 = 1 };

    Regulator selected_regulator;
    Pump selected_pump;
    float selected_regulator_pressure;
    float regulator_1_pressure;
    float regulator_2_pressure;

    PinState regulator_1_input;
    PinState regulator_2_input;

    uint32_t selected_pump_duty;

    PinState flow1;
    PinState flow2;

    PinState Sdc = PinState::ON;

    float pressure_1 = 0;
    float pressure_2 = 0;
    float pressure_3 = 0;
    float pressure_4 = 0;

    Actuators();

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