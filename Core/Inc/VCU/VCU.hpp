#pragma once
#include "state_machine.hpp"
#include "Comms.hpp"

class VCU {
   public:
    static inline VCU_SM* state_machine{};
    static inline uint8_t* VCU_state{};

    static void init();
    static void start();
    static void update();
};