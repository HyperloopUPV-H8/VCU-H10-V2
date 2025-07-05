#include "VCU/Comms.hpp"

#include "VCU/VCU.hpp"

void Comms::on_potencia_refri() {
    if (actuators->selected_pump == Actuators::Pump::PUMP_UNIDADES) {
        actuators->set_pump_1(actuators->selected_pump_duty);
    } else if (actuators->selected_pump == Actuators::Pump::PUMP_PLACAS) {
        actuators->set_pump_2(actuators->selected_pump_duty);
    }
}

void Comms::on_Set_regulator() {
    if (actuators->selected_regulator_pressure > 6) {
        actuators->selected_regulator_pressure = 6;
        // InfoWarning::InfoWarningTrigger("Cannot exceed 6 bar on regulator
        // pressure");
    }
    if (actuators->selected_regulator == Actuators::Regulator::REGULATOR_1) {
        actuators->regulator_1_pressure =
            actuators->selected_regulator_pressure;
        actuators->set_regulator_1(actuators->regulator_1_pressure);
    } else if (actuators->selected_regulator ==
               Actuators::Regulator::REGULATOR_2) {
        actuators->regulator_2_pressure =
            actuators->selected_regulator_pressure;
        actuators->set_regulator_2(actuators->regulator_2_pressure);
    }
}

void Comms::on_Enable_tapes() {
    brakes->tape_enable_status = PinState::ON;
    brakes->tape_enable_output.turn_on();
}

void Comms::on_Disable_tapes() {
    brakes->tape_enable_status = PinState::OFF;
    brakes->tape_enable_output.turn_off();
}

void Comms::on_brake() { brakes->brake(); }

void Comms::on_unbrake() { brakes->unbrake(); }

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



    lcu_tcp = new Socket(IPV4(VCU_IP),LOCAL_PORT, IPV4(LCU_IP), LCU_PORT);

    add_packets();
    add_orders();
}

void Comms::add_packets() {
    states = new HeapPacket(static_cast<uint16_t>(Packets_id::States),
                            VCU::general_state, VCU::operational_state);
    reeds = new HeapPacket(static_cast<uint16_t>(Packets_id::Reeds),
                           &brakes->reed1, &brakes->reed2, &brakes->reed3,
                           &brakes->reed4, &brakes->reed5, &brakes->reed6,
                           &brakes->reed7, &brakes->reed8, &brakes->All_reeds);
    flow = new HeapPacket(static_cast<uint16_t>(Packets_id::Flow),
                          &actuators->flow1, &actuators->flow2);
    regulator = new HeapPacket(static_cast<uint16_t>(Packets_id::Regulator),
                               &actuators->regulator_1_pressure,
                               &actuators->regulator_2_pressure);
    pressure = new HeapPacket(static_cast<uint16_t>(Packets_id::Pressure),
                              &actuators->pressure_1, &actuators->pressure_2,
                              &actuators->pressure_3, &actuators->pressure_4);
    tapes = new HeapPacket(
        static_cast<uint16_t>(Packets_id::Tapes), &brakes->tape_enable_status,&brakes->tape_emergency);

    sdc = new HeapPacket(
        static_cast<uint16_t>(Packets_id::Sdc), &actuators->Sdc);
}

void Comms::add_orders() {
    Potencia_refri = new HeapOrder(
        static_cast<uint16_t>(Orders_id::Potencia_refri_id), &on_potencia_refri,
        &actuators->selected_pump_duty, &actuators->selected_pump);
    Set_Regulator = new HeapOrder(
        static_cast<uint16_t>(Orders_id::Set_regulator_id), &on_Set_regulator,
        &actuators->selected_regulator_pressure,
        &actuators->selected_regulator);
    Enable_tapes = new HeapOrder(
        static_cast<uint16_t>(Orders_id::Enable_tapes_id), &on_Enable_tapes);
    Disable_tapes = new HeapOrder(
        static_cast<uint16_t>(Orders_id::Disable_tapes_id), &on_Disable_tapes);

    add_state_orders();
}

void Comms::add_state_orders() {
    StateOrder::set_socket(*control_station_tcp);
    Brake =
        new HeapStateOrder(static_cast<uint16_t>(Orders_id::Brake), &on_brake,
                           VCU::state_machine->GeneralStateMachine,
                           VCU_SM::GeneralStates::Operational);
    Unbrake =
        new HeapStateOrder(static_cast<uint16_t>(Orders_id::Unbrake),
                           &on_unbrake, VCU::state_machine->GeneralStateMachine,
                           VCU_SM::GeneralStates::Operational);
}

void Comms::send_packets() {
    control_station_udp->send_packet(*states);
    control_station_udp->send_packet(*reeds);
    control_station_udp->send_packet(*flow);
    control_station_udp->send_packet(*regulator);
    control_station_udp->send_packet(*pressure);
    control_station_udp->send_packet(*tapes);
}

void Comms::read_sensors() {
    actuators->read_regulators();
    actuators->read_pressure();
    actuators->read_flow();
    actuators->read_sdc();
    brakes->read_reeds();
    brakes->read_tape_emergency();
}

void Comms::update() {
    read_sensors();

    for (auto& pending : id_to_pending) {
        if (pending.second.control_station) {
            Socket_to_board[id_to_orders[pending.first].Board]->send_order(
                *id_to_orders[pending.first].order);
            id_to_timeout[pending.first] = Time::set_timeout(5000, [&]() {
                // InfoWarning::InfoWarningTrigger("Timeout for order to control
                // station");
                if (id_to_timeout.contains(pending.first)) {
                    Time::cancel_timeout(
                        id_to_timeout[pending.first]);  // No se si esto es
                                                        // valido
                    id_to_timeout.erase(pending.first);
                }
            });

        } else if (pending.second.board) {
            if (id_to_timeout.contains(pending.first)) {
                Time::cancel_timeout(id_to_timeout[pending.first]);
                id_to_timeout.erase(pending.first);
                auto it = id_to_flag.find(pending.first);
                if (it != id_to_flag.end()) {
                    *(it->second.first) = it->second.second;
                }
            }
        }
    }
}