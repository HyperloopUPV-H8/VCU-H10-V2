#ifndef SIM_ON
#include "main.h"

#include "lwip.h"
#endif

#include "ST-LIB.hpp"
#include "VCU/VCU.hpp"



int main(void) {
#ifdef SIM_ON
    SharedMemory::start();
#endif

    VCU vcu;

    while (1) {
        vcu.update();
    }
}

void Error_Handler(void) {
    ErrorHandler("HAL error handler triggered");
    while (1) {
    }
}
