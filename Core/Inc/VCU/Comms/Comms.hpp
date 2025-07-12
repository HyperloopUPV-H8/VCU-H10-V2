#pragma once
#include "ST-LIB.hpp"
#include "VCU/Actuators.hpp"
#include "VCU/Brakes.hpp"
#include "VCU/Comms/Comms.hpp"
#include "VCU/Leds.hpp"

class Comms {
   public:
    static inline Leds* leds;
    static inline Actuators* actuators;
    static inline Brakes* brakes;

    static inline uint32_t order_demonstration_bitfield{0};
    static constexpr uint8_t levitation_bit{0};
    static constexpr uint8_t propulsion_bit{1};
    static constexpr uint8_t charging_lv_bit{2};
    static constexpr uint8_t charging_hv_bit{3};
    static constexpr uint8_t horizontal_levitation_bit{4};
    static constexpr uint8_t booster_bit{5};
    static constexpr uint8_t brake_bit{6};
    static constexpr uint8_t close_contactors_bit{7};
    static constexpr uint8_t end_of_run_bit{8};

    static inline uint8_t hvscu_state{};
    static inline uint8_t pcu_state{};
    static inline uint8_t lcu_v_state{};
    static inline uint8_t lcu_h_state{};

    // -----------------Enums-----------------
    enum class Orders_id : uint16_t {
        Potencia_refri = 33,
        Set_Regulator = 34,
        Enable_tapes = 35,
        Disable_tapes = 36,

        Levitation = 37,
        Propulsion = 38,
        Charging_lv = 39,
        Charging_hv = 40,
        Horizontal_levitation = 41,
        Booster = 42,
        Brake = 43,
        Close_contactors = 44,
        End_of_run = 45,
        Stop_levitation = 46,
        Stop_propulsion = 47,
        Stop_charging_lv = 48,
        Stop_charging_hv = 49,
        Stop_horizontal_levitation = 50,
        Stop_booster = 51,
        Unbrake = 52,
        Open_contactors = 53,
        Forward_booster = 1788,
        Emergency_stop = 55,
        Reset_vehicle = 62,

        Runs = 56,
        SVPWM =  57,
        Stop_motor = 58,
        Current_control = 59,
        Speed_control = 60,
        Motor_brake = 61
    };

    enum class Packets_id : uint16_t {
        States = 249,
        Flow = 250,
        Reeds = 251,
        Regulator = 252,
        Pressure = 253,
        Tapes = 254,
        Sdc = 255,

        hvscu_state = 941,
        lcu_state = 63,
        pcu_state = 64
    };

    enum class External_ids : uint16_t {
        Levitation = 102,
        Propulsion = 103,
        Charging_lv = 104,
        Charging_hv = 105,
        Horizontal_levitation = 106,
        Booster = 1790,
        Close_contactors = 900,
        End_of_run = 110,
        Stop_levitation = 111,
        Stop_propulsion = 112,
        Stop_charging_lv = 113,
        Stop_charging_hv = 114,
        Stop_horizontal_levitation = 115,
        Stop_booster = 1789,
        Open_contactors = 901,
        Forward_booster = 1788,

        Runs = 1000,
        SVPWM =  1001,
        Stop_motor = 1002,
        Current_control = 1003,
        Speed_control = 1004,
        Motor_brake = 1005
    };

    enum class HVSCU_states : uint8_t { HVSCU_Opened = 0, HVSCU_Closed = 2 };

    enum class PCU_states : uint8_t {
        PCU_Stop_Propulsion = 0,
        PCU_Propulsion = 2
    };

    enum class LCU_states : uint8_t {
        LCU_Levitation = 3,
        LCU_Stop_Levitation = 0
    };

    enum class Booster_states : uint8_t {
        Booster_Disabled = 0,
        Booster_Enabled = 1,
    };

    // -----------------Flags-----------------
    static inline bool packet_sending{};
    static inline bool reading_sensors{};

    // Orders

    static inline bool levitation_flag{};
    static inline bool propulsion_flag{};
    static inline bool charging_lv_flag{};
    static inline bool charging_hv_flag{};
    static inline bool horizontal_levitation_flag{};
    static inline bool booster_flag{};

    static inline bool brake_flag{};
    static inline bool close_contactors_flag{};
    static inline bool end_of_run_flag{};

    static inline bool stop_levitation_flag{};
    static inline bool stop_propulsion_flag{};
    static inline bool stop_charging_lv_flag{};
    static inline bool stop_charging_hv_flag{};
    static inline bool stop_horizontal_levitation_flag{};
    static inline bool stop_booster_flag{};

    static inline bool unbrake_flag{};
    static inline bool open_contactors_flag{};

    static inline bool runs_flag{};
    static inline bool svpwm_flag{};
    static inline bool stop_motor_flag{};
    static inline bool current_control_flag{};
    static inline bool speed_control_flag{};
    static inline bool motor_brake_flag{};

    // Orders sent to boards

    static inline bool close_contactors_sent{};
    static inline bool open_contactors_sent{};

    static inline bool propulsion_sent{};
    static inline bool stop_propulsion_sent{};

    static inline bool levitation_sent{};
    static inline bool stop_levitation_sent{};

    static inline bool booster_sent{};
    static inline bool stop_booster_sent{};

    static inline bool runs_sent{};
    static inline bool svpwm_sent{};
    static inline bool stop_motor_sent{};
    static inline bool current_control_sent{};
    static inline bool speed_control_sent{};
    static inline bool motor_brake_sent{}; 

    // -----------------Order variables-----------------
    // PCU

    enum class Direction : uint8_t { FORWARD = 0, BACKWARDS = 1 };


    //Order 1 Runs
    static inline uint8_t run_id{};


    // Order 2 Start SVPWM // no movement
    static inline Direction motor_direction_1;
    static inline float max_voltage_1{};
    static inline float reference_voltage_1{};
    static inline float commutation_frequency_1{};
    static inline float modulation_frequency_1{};

    // Order 3  Stop motor
    //NO ARGS

    // Order 4 Send reference current
    static inline float reference_current_2{};
    static inline Direction motor_direction_2;
    static inline float max_voltage_2{};
    static inline float commutation_frequency_2{};
    static inline float modulation_frequency_2{};

    // Order 5 Send reference speed
    static inline Direction motor_direction_3{};
    static inline float reference_speed_3{};
    static inline float max_voltage_3{};
    static inline float commutation_frequency_3{};

    // Order 6 Brake using motor
    // NO ARGS


    // LCU
    static inline float levitation_distance{};

    // -----------------IP's/Ports-----------------
    static constexpr std::string CONTROL_STATION_IP = "192.168.0.9";

    static constexpr std::string VCU_IP = "192.168.1.3";
    static constexpr std::string PCU_IP = "192.168.1.5";
    static constexpr std::string HVSCU_IP = "192.168.1.7";
    static constexpr std::string BMSL_IP = "192.168.1.3";
    static constexpr std::string LCU_IP = "192.168.1.4";
    static constexpr std::string BCU_IP = "192.168.2.17";
    static constexpr std::string BLCU_IP = "192.168.1.3";

    static const uint32_t REMOTE_PORT = 50500;
    static const uint32_t JUANS_REMOTE_PORT = 50501;
    static const uint32_t CONTROL_STATION_UDP_PORT = 50400;
    static const uint32_t UDP_PORT = 50401;

    static const uint32_t PCU_PORT = 50501;
    static const uint32_t PCU_UDP_PORT = 50402;

    static const uint32_t HVSCU_PORT = 50502;
    static const uint32_t HVSCU_UDP_PORT = 50403;

    static const uint32_t BMSL_PORT = 50503;
    static const uint32_t BMSL_UDP_PORT = 50404;

    static const uint32_t LCU_PORT = 50504;
    static const uint32_t LCU_UDP_PORT = 50405;

    static const uint32_t BCU_PORT = 50505;

    static const uint32_t BLCU_PORT = 50506;
    static const uint32_t BLCU_UDP_PORT = 50406;

    // -----------------Sockets-----------------
    static inline ServerSocket* control_station_tcp{};
    static inline DatagramSocket* control_station_udp{};

    static inline Socket* pcu_tcp{};
    static inline DatagramSocket* pcu_udp{};

    static inline Socket* hvscu_tcp{};
    static inline DatagramSocket* hvscu_udp{};

    static inline Socket* bmsl_tcp{};
    static inline DatagramSocket* bmsl_udp{};

    static inline Socket* lcu_tcp{};
    static inline DatagramSocket* lcu_udp{};

    static inline Socket* bcu_tcp{};
    static inline DatagramSocket* bcu_udp{};

    static inline Socket* blcu_tcp{};
    static inline DatagramSocket* blcu_udp{};

    // -----------------Packets-----------------
    static inline HeapPacket* states{};
    static inline HeapPacket* reeds{};
    static inline HeapPacket* flow{};
    static inline HeapPacket* regulator{};
    static inline HeapPacket* pressure{};
    static inline HeapPacket* tapes{};
    static inline HeapPacket* sdc{};

    // Remote packets
    static inline HeapPacket* hvscu_state_packet{};
    static inline HeapPacket* lcu_state_packet{};
    static inline HeapPacket* pcu_state_packet{};

    // -----------------Orders-----------------
    static inline HeapOrder* Potencia_refri{};
    static inline HeapOrder* Set_Regulator{};
    static inline HeapOrder* Enable_tapes{};
    static inline HeapOrder* Disable_tapes{};

    static inline HeapOrder* levitation{};
    static inline HeapOrder* remote_levitation{};
    static inline HeapOrder* propulsion{};
    static inline HeapOrder* remote_propulsion{};
    static inline HeapOrder* charging_lv{};
    static inline HeapOrder* remote_charging_lv{};
    static inline HeapOrder* charging_hv{};
    static inline HeapOrder* remote_charging_hv{};
    static inline HeapOrder* horizontal_levitation{};
    static inline HeapOrder* remote_horizontal_levitation{};
    static inline HeapOrder* booster{};
    static inline HeapOrder* remote_booster{};

    static inline HeapOrder* brake{};
    static inline HeapOrder* close_contactors{};
    static inline HeapOrder* remote_close_contactors{};
    static inline HeapOrder* end_of_run{};
    static inline HeapOrder* remote_end_of_run{};

    static inline HeapOrder* stop_levitation{};
    static inline HeapOrder* remote_stop_levitation{};
    static inline HeapOrder* stop_propulsion{};
    static inline HeapOrder* remote_stop_propulsion{};
    static inline HeapOrder* stop_charging_lv{};
    static inline HeapOrder* remote_stop_charging_lv{};
    static inline HeapOrder* stop_charging_hv{};
    static inline HeapOrder* remote_stop_charging_hv{};
    static inline HeapOrder* stop_horizontal_levitation{};
    static inline HeapOrder* remote_stop_horizontal_levitation{};
    static inline HeapOrder* stop_booster{};
    static inline HeapOrder* remote_stop_booster{};

    static inline HeapOrder* unbrake{};
    static inline HeapOrder* open_contactors{};
    static inline HeapOrder* remote_open_contactors{};

    static inline HeapOrder* forward_booster_order{};
    static inline HeapOrder* emergency_stop{};

    static inline HeapOrder* runs{};
    static inline HeapOrder* remote_runs{};
    static inline HeapOrder* svpwm{};
    static inline HeapOrder* remote_svpwm{};
    static inline HeapOrder* stop_motor{};
    static inline HeapOrder* remote_stop_motor{};
    static inline HeapOrder* current_control{};
    static inline HeapOrder* remote_current_control{};
    static inline HeapOrder* speed_control{};
    static inline HeapOrder* remote_speed_control{};
    static inline HeapOrder* motor_brake{};
    static inline HeapOrder* remote_motor_brake{};

    

    // -----------------Functions-----------------
    static void init();
    static void start();
    static void add_packets();
    static void send_packets();
    static void add_orders();

    static void read_sensors();
    static void update();
    static void check_orders();

    static void on_brake();
    static void on_unbrake();
    static void on_potencia_refri();
    static void on_Set_regulator();
    static void on_Enable_tapes();
    static void on_Disable_tapes();

    // Callbacks

    static void levitation_callback();
    static void propulsion_callback();
    static void charging_lv_callback();
    static void charging_hv_callback();
    static void horizontal_levitation_callback();
    static void booster_callback();

    static void brake_callback();
    static void close_contactors_callback();
    static void end_of_run_callback();

    static void stop_levitation_callback();
    static void stop_propulsion_callback();
    static void stop_charging_lv_callback();
    static void stop_charging_hv_callback();
    static void stop_horizontal_levitation_callback();
    static void stop_booster_callback();

    static void unbrake_callback();
    static void open_contactors_callback();

    static void emergency_stop_callback();

    static void runs_callback();
    static void svpwm_callback();
    static void stop_motor_callback();
    static void current_control_callback();
    static void speed_control_callback();
    static void motor_brake_callback();

    // Check orders
    static void check_close_contactors_order();
    static void check_open_contactors_order();

    static void check_brake_order();
    static void check_unbrake_order();

    static void check_propulsion_order();
    static void check_stop_propulsion_order();

    static void check_levitation_order();
    static void check_stop_levitation_order();

    static void check_booster_order();
    static void check_stop_booster_order();

    static void check_runs_order();
    static void check_svpwm_order();
    static void check_stop_motor_order();
    static void check_current_control_order();
    static void check_speed_control_order();
    static void check_motor_brake_order();
};