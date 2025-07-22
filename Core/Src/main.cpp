#ifndef SIM_ON
#include "main.h"

#include "lwip.h"
#endif

#include "ST-LIB.hpp"
#include "VCU/VCU.hpp"

int main(void) {

    VCU::init();
    STLIB::start("00:00:00:00:01:03","192.168.1.3", "255.255.255.0");
    Time::set_timeout(4000, []() {
        VCU::start();
        VCU::state_machine->tetas = true;
    });

    while (1) {
        STLIB::update();
        VCU::update();
    }
}

void Error_Handler(void) {
    ErrorHandler("HAL error handler triggered");
    while (1) {
    }
}
