#include "VCU/Leds.hpp"

#include "VCU/Pinout.hpp"

Leds::Leds() {
    LedOperational = DigitalOutput(LED_OPERATIONAL_PIN);
    LedFault = DigitalOutput(LED_FAULT_PIN);
    LedCan = DigitalOutput(LED_CAN_PIN);
    LedSleep = DigitalOutput(LED_SLEEP_PIN);
    LedFlash = DigitalOutput(LED_FLASH_PIN);
}

void Leds::leds_connecting() {
    if (blinking_action_id != UNDEFINED_ID) {
        Time::unregister_low_precision_alarm(blinking_action_id);
        blinking_action_id = UNDEFINED_ID;
    }
    blinking_action_id = Time::register_low_precision_alarm(
        LINK_PERIOD_MS, [this]() { LedOperational.toggle(); });
}

void Leds::leds_operational() {
    if (blinking_action_id != UNDEFINED_ID) {
        Time::unregister_low_precision_alarm(blinking_action_id);
        blinking_action_id = UNDEFINED_ID;
    }
    LedOperational.turn_on();
}

void Leds::leds_fault() {
    if (blinking_action_id != UNDEFINED_ID) {
        Time::unregister_low_precision_alarm(blinking_action_id);
        blinking_action_id = UNDEFINED_ID;
    }
    blinking_action_id = Time::register_low_precision_alarm(
        LINK_PERIOD_MS, [this]() { LedFault.toggle(); });
}