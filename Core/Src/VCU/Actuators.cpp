#include "VCU/Actuators.hpp"

#include "VCU/Pinout.hpp"

Actuators::Actuators() {
    Regulator_out1 = PWM(REGULATOR_1_OUT_PIN);
    Regulator_out2 = PWM(REGULATOR_2_OUT_PIN);

    Regulator_in1 = DigitalSensor(REGULATOR_1_IN_PIN, &regulator_1_input);
    Regulator_in2 = DigitalSensor(REGULATOR_2_IN_PIN, &regulator_2_input);

    Pump_c1 = PWM(PUMP_C1_PIN);
    Pump_c2 = PWM(PUMP_C2_PIN);

    flow1_input = DigitalSensor(FLOW1_PIN, &flow1);
    flow2_input = DigitalSensor(FLOW2_PIN, &flow2);
    SDC_sensor = DigitalSensor(SDC_PIN, &Sdc);

    PresionAlta =
        LinearSensor<float>(PRESION_ALTA_PIN, 0.00763, 0.318, &pressure_1);
    PresionRegulador =
        LinearSensor<float>(PRESION_REGULADOR_PIN, 0.256, 0.375, &pressure_2);
    PresionFrenos =
        LinearSensor<float>(PRESION_FRENOS_PIN, 0.256, 0.375, &pressure_3);
    PresionCapsula = LinearSensor<float>(
        PRESION_CAPSULA_PIN, 1, 0, &pressure_4);  // Falta por caracterizar
}

void Actuators::init() {
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
    Regulator_out2.turn_on();
}

void Actuators::set_regulator_1(float value) {
    float regulator_value = static_cast<float>(
        value * (100.0 / 6));  // FAlta hacer la caracterizacion pero echale que
                               // al 100 deberia sacar 6 bares
    Regulator_out1.set_duty_cycle(regulator_value);
}

void Actuators::set_regulator_2(float value) {
    float regulator_value = static_cast<float>(value * (100.0 / 6));
    Regulator_out2.set_duty_cycle(regulator_value);
}

void Actuators::set_pump_1(uint32_t value) { Pump_c1.set_duty_cycle(value); }
void Actuators::set_pump_2(uint32_t value) { Pump_c2.set_duty_cycle(value); }

void Actuators::read_regulators() {
    Regulator_in1.read();
    Regulator_in2.read();
}

void Actuators::read_pressure() {
    PresionAlta.read();
    PresionRegulador.read();
    PresionFrenos.read();
    // PresionCapsula.read();
}

void Actuators::read_flow() {
    flow1_input.read();
    flow2_input.read();
}

void Actuators::read_sdc() { SDC_sensor.read(); }