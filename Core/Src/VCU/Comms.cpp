#include "VCU/Comms.hpp"

#include "VCU/VCU.hpp"

//------------------------CALLBACKS------------------------

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

void Comms::brake_callback() { brakes->brake(); }

void Comms::unbrake_callback() { brakes->unbrake(); }

void Comms::close_contactors_callback() { close_contactors_flag = true; }

void Comms::open_contactors_callback() { open_contactors_flag = true; }

void Comms::levitation_callback() { levitation_flag = true; }

void Comms::stop_levitation_callback() { stop_levitation_flag = true; }

void Comms::booster_callback() { booster_flag = true; }

void Comms::stop_booster_callback() { stop_booster_flag = true; }

void Comms::emergency_stop_callback() {
    VCU::state_machine->GeneralStateMachine.force_change_state(
        VCU_SM::GeneralStates::Fault);
}

//------------------------------------------------------

void Comms::init() {
    leds = new Leds();
    actuators = new Actuators();
    brakes = new Brakes();
}

void Comms::start() {
    control_station_tcp = new ServerSocket(IPV4(VCU_IP), REMOTE_PORT);

    control_station_udp =
        new DatagramSocket(IPV4(VCU_IP), CONTROL_STATION_UDP_PORT,
                           IPV4(CONTROL_STATION_IP), CONTROL_STATION_UDP_PORT);

    lcu_tcp = new Socket(IPV4(VCU_IP), LCU_PORT, IPV4(LCU_IP), REMOTE_PORT);

    /* hvscu_tcp =
        new Socket(IPV4(VCU_IP), HVSCU_PORT, IPV4(HVSCU_IP), REMOTE_PORT);

    hvscu_udp = new DatagramSocket(IPV4(VCU_IP), HVSCU_UDP_PORT, IPV4(HVSCU_IP),
                                   HVSCU_UDP_PORT); */

    lcu_udp = new DatagramSocket(IPV4(VCU_IP), LCU_UDP_PORT, IPV4(LCU_IP),
                                 LCU_UDP_PORT);

    bcu_tcp = new Socket(IPV4(VCU_IP), BCU_PORT, IPV4(BCU_IP), JUANS_REMOTE_PORT);

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
    tapes =
        new HeapPacket(static_cast<uint16_t>(Packets_id::Tapes),
                       &brakes->tape_enable_status, &brakes->tape_emergency);

    sdc =
        new HeapPacket(static_cast<uint16_t>(Packets_id::Sdc), &actuators->Sdc);

    // Remote packets

    hvscu_state_packet = new HeapPacket(
        static_cast<uint16_t>(Packets_id::hvscu_state), &hvscu_state);

    lcu_state_packet =
        new HeapPacket(static_cast<uint16_t>(Packets_id::lcu_state),
                       &lcu_v_state, &lcu_h_state);
}

void Comms::add_orders() {
    Potencia_refri = new HeapOrder(
        static_cast<uint16_t>(Orders_id::Potencia_refri), &on_potencia_refri,
        &actuators->selected_pump_duty, &actuators->selected_pump);

    Set_Regulator = new HeapOrder(
        static_cast<uint16_t>(Orders_id::Set_Regulator), &on_Set_regulator,
        &actuators->selected_regulator_pressure,
        &actuators->selected_regulator);

    Enable_tapes = new HeapOrder(static_cast<uint16_t>(Orders_id::Enable_tapes),
                                 &on_Enable_tapes);

    Disable_tapes = new HeapOrder(
        static_cast<uint16_t>(Orders_id::Disable_tapes), &on_Disable_tapes);

    brake =
        new HeapOrder(static_cast<uint16_t>(Orders_id::Brake), &brake_callback);

    unbrake = new HeapOrder(static_cast<uint16_t>(Orders_id::Unbrake),
                            &unbrake_callback);

    close_contactors =
        new HeapOrder(static_cast<uint16_t>(Orders_id::Close_contactors),
                      &close_contactors_callback);

    open_contactors =
        new HeapOrder(static_cast<uint16_t>(Orders_id::Open_contactors),
                      &open_contactors_callback);

    levitation = new HeapOrder(static_cast<uint16_t>(Orders_id::Levitation),
                               &levitation_callback, &levitation_distance);

    stop_levitation =
        new HeapOrder(static_cast<uint16_t>(Orders_id::Stop_levitation),
                      &stop_levitation_callback);

    booster = new HeapOrder(static_cast<uint16_t>(Orders_id::Booster),
                            &booster_callback);

    stop_booster = new HeapOrder(static_cast<uint16_t>(Orders_id::Stop_booster),
                                 &stop_booster_callback);

    forward_booster_order =
        new HeapOrder(static_cast<uint16_t>(Orders_id::Forward_booster));

    emergency_stop =
        new HeapOrder(static_cast<uint16_t>(Orders_id::Emergency_stop),
                      &emergency_stop_callback);
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

void Comms::check_close_contactors_order() {
    /* if (close_contactors_flag) {
        if (*VCU::operational_state == VCU_SM::OperationalStates::Idle) {
            if (!close_contactors_sent) {
                hvscu_tcp->send_order(*close_contactors);
                close_contactors_sent = true;
            } else {
                if (hvscu_state ==
                    static_cast<uint8_t>(Boards_states::HVSCU_Closed)) {
                    actuators->contactors_closed = true;
                    close_contactors_flag = false;
                    close_contactors_sent = false;
                } else {
                    return;
                }
            }
        } else {
            WARNING("Cannot close contactors in this state");
            close_contactors_flag = false;
        }
    } */
    if (close_contactors_flag) {
        if (*VCU::operational_state == VCU_SM::OperationalStates::Idle) {
            actuators->contactors_closed = true;
            close_contactors_flag = false;
        }
    } else {
        WARNING("Cannot close contactors in this state");
        close_contactors_flag = false;
    }
}

void Comms::check_levitation_order() {
    if (levitation_flag) {
        if (*VCU::operational_state == VCU_SM::OperationalStates::Ready ||
            *VCU::operational_state ==
                VCU_SM::OperationalStates::Demonstration) {
            static uint8_t timeout_id = -1;

            if (!levitation_sent) {
                lcu_tcp->send_order(*levitation);
                levitation_sent = true;
            } else {
                if (lcu_v_state ==
                    static_cast<uint8_t>(LCU_states::LCU_Levitation)) {
                    Time::cancel_timeout(timeout_id);
                    timeout_id = -1;
                    order_demonstration_bitfield |= (1UL << levitation_bit);
                    levitation_flag = false;
                    levitation_sent = false;
                } else {
                    if (timeout_id == -1) {
                        timeout_id = Time::set_timeout(100, [&]() {
                            VCU::state_machine->GeneralStateMachine
                                .force_change_state(
                                    VCU_SM::GeneralStates::Fault);
                        });
                    }
                }
            }
        } else {
            WARNING("Cannot levitate in this state");
            levitation_flag = false;
        }
    }
}

void Comms::check_stop_levitation_order() {
    if (stop_levitation_flag) {
        if (*VCU::operational_state ==
            VCU_SM::OperationalStates::Demonstration) {
            static uint8_t timeout_id = -1;

            if (!stop_levitation_sent) {
                lcu_tcp->send_order(*stop_levitation);
                stop_levitation_sent = true;
            } else {
                if (lcu_v_state ==
                    static_cast<uint8_t>(LCU_states::LCU_Stop_Levitation)) {
                    Time::cancel_timeout(timeout_id);
                    timeout_id = -1;
                    order_demonstration_bitfield &= ~(1UL << levitation_bit);
                    stop_levitation_flag = false;
                    stop_levitation_sent = false;
                } else {
                    if (timeout_id == -1) {
                        timeout_id = Time::set_timeout(100, [&]() {
                            VCU::state_machine->GeneralStateMachine
                                .force_change_state(
                                    VCU_SM::GeneralStates::Fault);
                        });
                    }
                }
            }
        } else {
            WARNING("Cannot stop levitating in this state");
            stop_levitation_flag = false;
        }
    }
}

void Comms::check_booster_order() {
    if (booster_flag) {
        if (*VCU::operational_state == VCU_SM::OperationalStates::Ready ||
            *VCU::operational_state ==
                VCU_SM::OperationalStates::Demonstration) {
            static uint8_t timeout_id = -1;

            if (!booster_sent) {
                lcu_tcp->send_order(*booster);
                booster_sent = true;
            } else {
                if (lcu_h_state ==
                    static_cast<uint8_t>(Booster_states::Booster_Enabled)) {
                    Time::cancel_timeout(timeout_id);
                    timeout_id = -1;
                    order_demonstration_bitfield |= (1UL << booster_bit);
                    bcu_tcp->send_order(*forward_booster_order);
                    booster_flag = false;
                    booster_sent = false;
                } else {
                    if (timeout_id == -1) {
                        timeout_id = Time::set_timeout(100, [&]() {
                            VCU::state_machine->GeneralStateMachine
                                .force_change_state(
                                    VCU_SM::GeneralStates::Fault);
                        });
                    }
                }
            }
        } else {
            WARNING("Cannot boost in this state");
            booster_flag = false;
        }
    }
}

void Comms::check_stop_booster_order() {
    if (stop_booster_flag) {
        if (*VCU::operational_state ==
            VCU_SM::OperationalStates::Demonstration) {
            static uint8_t timeout_id = -1;

            if (!stop_booster_sent) {
                lcu_tcp->send_order(*stop_booster);
                stop_booster_sent = true;
            } else {
                if (lcu_h_state ==
                    static_cast<uint8_t>(Booster_states::Booster_Disabled)) {
                    Time::cancel_timeout(timeout_id);
                    timeout_id = -1;
                    order_demonstration_bitfield &= ~(1UL << booster_bit);
                    stop_booster_flag = false;
                    stop_booster_sent = false;
                } else {
                    if (timeout_id == -1) {
                        timeout_id = Time::set_timeout(100, [&]() {
                            VCU::state_machine->GeneralStateMachine
                                .force_change_state(
                                    VCU_SM::GeneralStates::Fault);
                        });
                    }
                }
            }
        } else {
            WARNING("Cannot stop boosting in this state");
            stop_booster_flag = false;
        }
    }
}

void Comms::check_orders() {
    check_close_contactors_order();

    if (open_contactors_flag) {
        // hvscu_tcp->send_order(*open_contactors);
        actuators->contactors_closed = false;
        stop_levitation_flag = true;

        open_contactors_flag = false;
    }

    check_levitation_order();
    check_stop_levitation_order();
    check_booster_order();
    check_stop_booster_order();
}

void Comms::update() {
    check_orders();
    read_sensors();
}