#include "VCU/Comms.hpp"

void Comms::on_potencia_refri() {
    if (actuators->selected_pump == Actuators::Pump::PUMP_UNIDADES) {
        actuators->set_pump_1(actuators->selected_pump_duty);
    } else if (actuators->selected_pump == Actuators::Pump::PUMP_PLACAS) {
        actuators->set_pump_2(actuators->selected_pump_duty);
    }
}

void Comms::init() {
    leds = new Leds();
    actuators = new Actuators();
    brakes = new Brakes();
}

void Comms::start() {
    control_station_tcp = new ServerSocket(IPV4(VCU_IP), LOCAL_PORT);
    control_station_udp =
        new DatagramSocket(IPV4(VCU_IP), CONTROL_STATION_UDP_PORT,
                           IPV4(CONTROL_SATION_IP), CONTROL_STATION_UDP_PORT);

    add_packets();
    add_orders();
}

void Comms::add_packets() {}

void Comms::add_orders() {
    Potencia_refri = new HeapOrder(
        static_cast<uint16_t>(Orders_id::Potencia_refri_id), &on_potencia_refri,
        &actuators->selected_pump_duty, &actuators->selected_pump);
}