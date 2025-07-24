#pragma once
#include "ST-LIB.hpp"


class Leds {
    private:
    static constexpr uint32_t LINK_PERIOD_MS{500};
    static constexpr uint8_t UNDEFINED_ID{std::numeric_limits<uint8_t>::max()};

    DigitalOutput LedOperational;
    DigitalOutput LedFault;
    DigitalOutput LedCan;   //Ver que hacer con estos leds
    DigitalOutput LedSleep;
    DigitalOutput LedFlash;

    uint8_t blinking_action_id{UNDEFINED_ID};

    public:
    Leds();
    void leds_connecting();
    void leds_operational();
    void leds_fault();
    
};  
