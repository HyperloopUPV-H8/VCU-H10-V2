#pragma once
#include "Pinout.hpp"

namespace Pinout {
Pin& led_operational_pin(LED_OPERATIONAL_PIN);
Pin& led_fault_pin(LED_FAULT_PIN);
Pin& led_can_pin(LED_CAN_PIN);
Pin& led_sleep_pin(LED_SLEEP_PIN);
Pin& led_flash_pin(LED_FLASH_PIN);
};