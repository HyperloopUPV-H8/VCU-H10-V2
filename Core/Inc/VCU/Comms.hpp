#pragma once
#include "ST-LIB.hpp"
#include "VCU/Actuators.hpp"
#include "VCU/Brakes.hpp"
#include "VCU/Comms.hpp"
#include "VCU/Leds.hpp"

class Comms {
   public:

    static inline Leds* leds;
    static inline Actuators* actuators;
    static inline Brakes* brakes;

    // -----------------Enums-----------------
    enum class Orders_id : uint16_t {
        // IDs inventadas excepto las de id con 200
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

    enum class Packets_id : uint16_t {
        States = 249,
        Flow = 250,
        Reeds = 251,
        Regulator = 252,
        Pressure = 253,
        Tapes_input = 254,
        Tapes_enable = 255
    };

    enum class Boards : uint8_t { PCU, HVSCU, BMSL, LCU, BLCU, BCU };

    // -----------------Structs-----------------
    struct Flags_ready {
        bool requested_levitation_active;
        bool requested_propulsion_active;
        bool requested_charging_LV_battery;
        bool requested_horizontal_levitation_active;
        bool requested_enable_booster;

        Flags_ready()
            : requested_levitation_active(false),
              requested_propulsion_active(false),
              requested_charging_LV_battery(false),
              requested_horizontal_levitation_active(false),
              requested_enable_booster(false) {}
    };

    struct PendingOrder {
        bool control_station;
        bool board;
    };

    struct BoardOrder {
        Boards Board;
        HeapStateOrder* order;
    };

    // -----------------Flags-----------------
    static inline Flags_ready flags_ready;
    static inline bool requested_open_contactors{};
    static inline bool requested_close_contactors{};
    static inline bool requested_end_of_run{};
    static inline bool packet_sending{};
    static inline bool reading_sensors{};

    // -----------------IP's/Ports-----------------
    static constexpr std::string CONTROL_SATION_IP = "192.168.0.9";

    static constexpr std::string VCU_IP = "192.168.1.3";
    static constexpr std::string PCU_IP = "192.168.1.3";
    static constexpr std::string HVSCU_IP = "192.168.1.3";
    static constexpr std::string BMSL_IP = "192.168.1.3";
    static constexpr std::string LCU_IP = "192.168.1.3";
    static constexpr std::string BLCU_IP = "192.168.1.3";

    static const uint16_t LOCAL_PORT = 50500;
    static const uint16_t CONTROL_STATION_UDP_PORT = 50400;
    static const uint16_t UDP_PORT = 50401;

    static const uint16_t PCU_PORT = 50501;
    static const uint16_t PCU_UDP_PORT = 50402;

    static const uint16_t HVSCU_PORT = 50502;
    static const uint16_t HVSCU_UDP_PORT = 50403;

    static const uint16_t BMSL_PORT = 50503;
    static const uint16_t BMSL_UDP_PORT = 50404;

    static const uint16_t LCU_PORT = 50504;
    static const uint16_t LCU_UDP_PORT = 50405;

    static const uint16_t BLCU_PORT = 50505;
    static const uint16_t BLCU_UDP_PORT = 50406;

    // -----------------Sockets-----------------
    static inline ServerSocket* control_station_tcp{};
    static inline DatagramSocket* control_station_udp{};

    static inline Socket pcu_tcp{};
    static inline DatagramSocket pcu_udp{};

    static inline Socket hvscu_tcp{};
    static inline DatagramSocket hvscu_udp{};

    static inline Socket bmsl_tcp{};
    static inline DatagramSocket bmsl_udp{};

    static inline Socket lcu_tcp{};
    static inline DatagramSocket lcu_udp{};

    static inline Socket blcu_tcp{};
    static inline DatagramSocket blcu_udp{};

    // -----------------Packets-----------------
    static inline HeapPacket* states{};
    static inline HeapPacket* reeds{};
    static inline HeapPacket* flow{};
    static inline HeapPacket* regulator{};
    static inline HeapPacket* pressure{};
    // static inline HeapPacket* Tapes{};
    static inline HeapPacket* tapes_enabled{};

    /* static std::vector<HeapPacket*> packets{};  // Lo que mando a la gui
    struct OrderTriggers {
        Boards board;
        bool* flag;
        HeapOrder* order;
    };
    static std::vector<OrderTriggers> order_triggers; */

    // -----------------Orders-----------------
    static inline HeapOrder* Potencia_refri{};
    static inline HeapOrder* Set_Regulator{};
    static inline HeapOrder* Enable_tapes{};
    static inline HeapOrder* Disable_tapes{};

    static inline HeapStateOrder* Open_Contactors{};
    static inline HeapStateOrder* Close_Contactors{};
    static inline HeapStateOrder* Unbrake{};
    static inline HeapStateOrder* Brake{};
    static inline HeapStateOrder* EndOfRun{};

    static inline HeapStateOrder* Levitation_Active{};
    static inline HeapStateOrder* Propulsion_Active{};
    static inline HeapStateOrder* Charging_LV_Battery{};
    static inline HeapStateOrder* Enable_Booster{};
    static inline HeapStateOrder* Levitation_Inactive{};
    static inline HeapStateOrder* Propulsion_Inactive{};
    static inline HeapStateOrder* Charging_LV_Battery_Inactive{};
    static inline HeapStateOrder* Disable_booster{};

    // -----------------Functions-----------------
    static void init();
    static void start();
    static void add_packets();
    static void send_packets();
    static void add_orders();
    static void add_state_orders();

    static void update();
    static bool connected();

    static void on_brake();
    static void on_unbrake();
    static void on_potencia_refri();
    static void on_Set_regulator();
    static void on_Enable_tapes();
    static void on_Disable_tapes();

    // Funciones de las flags, y las que se mandan a otras placas, cambiar y tal
    // al gusto:
    static void on_levitation_active();
    static void on_propulsion_active();
    static void on_charging_LV_battery();
    static void on_horizontal_levitation_active();
    static void on_enable_booster();

    static void on_levitation_inactive();
    static void on_propulsion_inactive();
    static void on_charging_LV_battery_inactive();
    static void on_horizontal_levitation_inactive();
    static void on_disable_booster();

    static void on_open_contactors();
    static void on_close_contactors();

    static void on_end_of_run();

    // -----------------Unordered maps-----------------
    static inline std::unordered_map<Boards, Socket*> Socket_to_board{};

    static inline std::unordered_map<Boards, string> Board_to_ip{
        {Boards::PCU, PCU_IP},
        {Boards::HVSCU, HVSCU_IP},
        {Boards::BMSL, BMSL_IP},
        {Boards::LCU, LCU_IP},
        {Boards::BLCU, BLCU_IP}};

    static inline std::unordered_map<Orders_id, PendingOrder> id_to_pending{
        {Orders_id::Levitation_active, {false, false}},
        {Orders_id::Propulsion_active, {false, false}},
        {Orders_id::Charging_LV_battery, {false, false}},
        {Orders_id::Enable_booster, {false, false}},
        {Orders_id::Open_contactors, {false, false}},
        // {Orders_id::Brake, {false, false}},
        {Orders_id::Close_contactors, {false, false}},
        // {Orders_id::Unbrake, {false, false}},
        {Orders_id::Levitation_inactive, {false, false}},
        {Orders_id::Propulsion_inactive, {false, false}},
        {Orders_id::Charging_LV_battery_inactive, {false, false}},
        {Orders_id::Disable_booster, {false, false}}};

    static inline std::unordered_map<Orders_id, std::pair<bool*, bool>>
        id_to_flag{
            {Orders_id::Levitation_active,
             {&flags_ready.requested_levitation_active, true}},
            {Orders_id::Propulsion_active,
             {&flags_ready.requested_propulsion_active, true}},
            {Orders_id::Charging_LV_battery,
             {&flags_ready.requested_charging_LV_battery, true}},
            {Orders_id::Enable_booster,
             {&flags_ready.requested_enable_booster, true}},
            {Orders_id::Open_contactors, {&requested_open_contactors, true}},
            // {Orders_id::Brake, {&requested_brake, true}},
            {Orders_id::Levitation_inactive,
             {&flags_ready.requested_levitation_active, false}},
            {Orders_id::Propulsion_inactive,
             {&flags_ready.requested_propulsion_active, false}},
            {Orders_id::Charging_LV_battery_inactive,
             {&flags_ready.requested_charging_LV_battery, false}},
            {Orders_id::Disable_booster,
             {&flags_ready.requested_enable_booster, false}},
            {Orders_id::Close_contactors, {&requested_close_contactors, true}},
            // {Orders_id::Unbrake, {&requested_unbrake, true}}
            // {Orders_id::EndOfRun_id, {&requested_end_of_run, true}}
        };

    static inline std::unordered_map<Orders_id, BoardOrder> id_to_orders{
        {Orders_id::Levitation_active, {Boards::LCU, Levitation_Active}},
        {Orders_id::Propulsion_active, {Boards::PCU, Propulsion_Active}},
        {Orders_id::Charging_LV_battery, {Boards::BMSL, Charging_LV_Battery}},
        {Orders_id::Enable_booster, {Boards::BCU, Enable_Booster}},
        {Orders_id::Open_contactors, {Boards::HVSCU, Open_Contactors}},
        // {Orders_id::Brake, {Boards::, Brake}},
        {Orders_id::Close_contactors, {Boards::HVSCU, Close_Contactors}},
        // {Orders_id::Unbrake, {Boards::PCU, Unbrake}},
        {Orders_id::Levitation_inactive, {Boards::LCU, Levitation_Inactive}},
        {Orders_id::Propulsion_inactive, {Boards::PCU, Propulsion_Inactive}},
        {Orders_id::Charging_LV_battery_inactive,
         {Boards::BMSL, Charging_LV_Battery_Inactive}},
        {Orders_id::Disable_booster, {Boards::BCU, Disable_booster}}};

    static inline std::unordered_map<Orders_id, uint8_t> id_to_timeout{};
};