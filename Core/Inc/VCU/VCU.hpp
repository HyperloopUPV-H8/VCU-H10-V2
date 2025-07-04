#pragma once
#include "Comms.hpp"
#include "state_machine.hpp"

class VCU {
    static void send_packets();

   public:
    static inline VCU_SM* state_machine{};
    static inline uint8_t* general_state{};
    static inline uint8_t* operational_state{};

    static void init();
    static void start();
    static void update();
};