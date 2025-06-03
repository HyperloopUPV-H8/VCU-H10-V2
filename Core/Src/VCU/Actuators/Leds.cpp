#pragma once
#include "Leds.hpp"

namespace Actuators {

    void Leds::leds_connecting() {
        if (blinking_action_id != UNDEFINED_ID) {
            Time::unregister_low_precision_alarm(blinking_action_id);
            blinking_action_id = UNDEFINED_ID;
        }
        blinking_action_id = Time::register_low_precision_alarm(LINK_PERIOD_MS,[this]() {
            LedOperational.toggle();
        });
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
        blinking_action_id = Time::register_low_precision_alarm(LINK_PERIOD_MS,[this]() {
            LedFault.toggle();
        });
    }



};