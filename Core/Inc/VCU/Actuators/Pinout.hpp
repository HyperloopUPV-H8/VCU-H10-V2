#pragma once
#include "ST-LIB.hpp"

#define LED_OPERATIONAL_PIN PG8
#define LED_FAULT_PIN PG7
#define LED_CAN_PIN PG6
#define LED_SLEEP_PIN PG4
#define LED_FLASH_PIN PG5


namespace Pinout {
    extern Pin& led_operational_pin;
    extern Pin& led_fault_pin;
    extern Pin& led_can_pin;
    extern Pin& led_sleep_pin;
    extern Pin& led_flash_pin;
};