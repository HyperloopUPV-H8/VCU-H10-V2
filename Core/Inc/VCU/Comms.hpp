#pragma once
#include "ST-LIB.hpp"

class Comms {
   public:

    enum Orders_id : uint16_t {
        Open_contactors = 1000,
        Close_contactors = 1001,
        Unbrake = 201,
        Brake = 200,
        Potencia_refri_id = 202,
        Set_regulator_id = 203,
        Enable_tapes_id = 204,
        Disable_tapes_id = 205,
        EndOfRun_id = 1002,
        Levitation_active = 1003,
        Propulsion_active = 1004,
        Charging_LV_battery = 1005,
        Enable_booster = 1006,
        Levitation_inactive = 1007,
        Propulsion_inactive = 1008,
        Charging_LV_battery_inactive = 1009,
        Disable_booster = 1010
    };

    enum Packets_id : uint16_t {
        Flow = 250,
        Reeds = 251,
        Regulator = 252,
        Pressure = 253,
        Tapes_input = 254,
        Tapes_enable = 255
    };

    // -----------------IP's/Ports-----------------
    static constexpr std::string CONTROL_SATION_IP = "192.168.0.9";
    static constexpr std::string VCU_IP = "192.168.1.3";

    static const uint16_t LOCAL_PORT = 50500;
    static const uint16_t CONTROL_STATION_UDP_PORT = 50400;


    // -----------------Sockets-----------------
    static inline ServerSocket* control_station_tcp{};
    static inline DatagramSocket* control_station_udp{};

    // -----------------Functions-----------------
    static void init();
    static void add_packets();
    static void add_orders();
    
};