#pragma once
#include "ST-LIB.hpp"

#define LED_OPERATIONAL_PIN PG8
#define LED_FAULT_PIN PG7
#define LED_CAN_PIN PG6
#define LED_SLEEP_PIN PG4
#define LED_FLASH_PIN PG5

#define REED1_PIN PD11
#define REED2_PIN PD10
#define REED3_PIN PG4
#define REED4_PIN PG3
#define REED5_PIN PD8
#define REED6_PIN PD9
#define REED7_PIN PB15
#define REED8_PIN PB14

#define REGULATOR_1_IN_PIN PA4
#define REGULATOR_2_IN_PIN PD12
#define REGULATOR_1_OUT_PIN PB9
#define REGULATOR_2_OUT_PIN PD13

#define BRAKE_PIN PE7
#define SDC_PIN PA12
#define PUMP_C1_PIN PE9
#define PUMP_C2_PIN PE11
#define TAPE_E_PIN PG1
#define FLOW1_PIN PE13
#define FLOW2_PIN PE14
#define TAPE_PIN PE0

#define PRESION_ALTA_PIN PB0
#define PRESION_REGULADOR_PIN PB1
#define PRESION_FRENOS_PIN PF11
#define PRESION_CAPSULA_PIN PF12

namespace Pinout {
    extern Pin& led_operational_pin;
    extern Pin& led_fault_pin;
    extern Pin& led_can_pin;
    extern Pin& led_sleep_pin;
    extern Pin& led_flash_pin;

    extern Pin& reed1_pin;
    extern Pin& reed2_pin;
    extern Pin& reed3_pin;
    extern Pin& reed4_pin;
    extern Pin& reed5_pin;
    extern Pin& reed6_pin;
    extern Pin& reed7_pin;
    extern Pin& reed8_pin;

    extern Pin& regulator_1_in_pin;
    extern Pin& regulator_2_in_pin;
    extern Pin& regulator_1_out_pin;
    extern Pin& regulator_2_out_pin;

    extern Pin& Brake_pin;
    extern Pin& SDC_pin;
    extern Pin& Pump_c1_pin;
    extern Pin& Pump_c2_pin;
    extern Pin& TapeE_pin;
    extern Pin& Flow1_pin;
    extern Pin& Flow2_pin;
    extern Pin& Tape_pin;

    extern Pin& Presion_alta_pin;
    extern Pin& Presion_regulador_pin;
    extern Pin& Presion_frenos_pin;
    extern Pin& Presion_capsula_pin;

};