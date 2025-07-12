#include "VCU/Comms/Comms.hpp"

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

void Comms::brake_callback() { brake_flag = true; }

void Comms::unbrake_callback() { unbrake_flag = true; }

void Comms::close_contactors_callback() { close_contactors_flag = true; }

void Comms::open_contactors_callback() { open_contactors_flag = true; }

void Comms::levitation_callback() { levitation_flag = true; }

void Comms::stop_levitation_callback() { stop_levitation_flag = true; }

void Comms::booster_callback() { booster_flag = true; }

void Comms::stop_booster_callback() { stop_booster_flag = true; }

void Comms::emergency_stop_callback() {
    ProtectionManager::fault_and_propagate();
}

void Comms::runs_callback() { runs_flag = true; }
void Comms::svpwm_callback() { svpwm_flag = true; }
void Comms::stop_motor_callback() { stop_motor_flag = true; }
void Comms::current_control_callback() { current_control_flag = true; }
void Comms::speed_control_callback() { speed_control_flag = true; }
void Comms::motor_brake_callback() { motor_brake_flag = true; }

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

    lcu_udp = new DatagramSocket(IPV4(VCU_IP), LCU_UDP_PORT, IPV4(LCU_IP),
                                 LCU_UDP_PORT);

    hvscu_tcp =
        new Socket(IPV4(VCU_IP), HVSCU_PORT, IPV4(HVSCU_IP), REMOTE_PORT);

    hvscu_udp = new DatagramSocket(IPV4(VCU_IP), HVSCU_UDP_PORT, IPV4(HVSCU_IP),
                                   HVSCU_UDP_PORT);

    bcu_tcp =
        new Socket(IPV4(VCU_IP), BCU_PORT, IPV4(BCU_IP), JUANS_REMOTE_PORT);

    pcu_tcp = new Socket(IPV4(VCU_IP), PCU_PORT, IPV4(PCU_IP), REMOTE_PORT);

    pcu_udp = new DatagramSocket(IPV4(VCU_IP), PCU_UDP_PORT, IPV4(PCU_IP),
                                 PCU_UDP_PORT);

    add_packets();
    add_orders();
}

void Comms::add_packets() {
    states = new HeapPacket(static_cast<uint16_t>(Packets_id::States),
                            VCU::general_state, VCU::operational_state);
    reeds = new HeapPacket(static_cast<uint16_t>(Packets_id::Reeds),
                           &brakes->reed1, &brakes->reed2, &brakes->reed3,
                           &brakes->reed4, &brakes->All_reeds);
    flow = new HeapPacket(static_cast<uint16_t>(Packets_id::Flow),
                          &actuators->flow1, &actuators->flow2);
    regulator = new HeapPacket(static_cast<uint16_t>(Packets_id::Regulator),
                               &actuators->regulator_1_pressure,
                               &actuators->regulator_2_pressure);
    pressure = new HeapPacket(
        static_cast<uint16_t>(Packets_id::Pressure), &actuators->pressure_high,
        &actuators->pressure_regulator, &actuators->pressure_brakes,
        &actuators->pressure_capsule);
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

    pcu_state_packet = new HeapPacket(
        static_cast<uint16_t>(Packets_id::pcu_state), &pcu_state);
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

    //-----------------------------------------------------------------------------

    remote_close_contactors =
        new HeapOrder(static_cast<uint16_t>(External_ids::Close_contactors),
                      &close_contactors_callback);

    remote_open_contactors =
        new HeapOrder(static_cast<uint16_t>(External_ids::Open_contactors),
                      &open_contactors_callback);

    remote_levitation =
        new HeapOrder(static_cast<uint16_t>(External_ids::Levitation),
                      &levitation_callback, &levitation_distance);

    remote_stop_levitation =
        new HeapOrder(static_cast<uint16_t>(External_ids::Stop_levitation),
                      &stop_levitation_callback);

    remote_booster = new HeapOrder(static_cast<uint16_t>(External_ids::Booster),
                                   &booster_callback);

    remote_stop_booster =
        new HeapOrder(static_cast<uint16_t>(External_ids::Stop_booster),
                      &stop_booster_callback);

    //-----------------------------------------------------------------------------

    close_contactors =
        new HeapOrder(static_cast<uint16_t>(Orders_id::Close_contactors),
                      &close_contactors_callback);

    open_contactors =
        new HeapOrder(static_cast<uint16_t>(Orders_id::Open_contactors),
                      &open_contactors_callback);

    /*  propulsion = new HeapOrder(static_cast<uint16_t>(Orders_id::Propulsion),
                                &propulsion_callback, &levitation_distance);

     stop_propulsion =
         new HeapOrder(static_cast<uint16_t>(Orders_id::Stop_propulsion),
                       &stop_propulsion_callback); */

    runs = new HeapOrder(static_cast<uint16_t>(Orders_id::Runs), &runs_callback,
                         &run_id);

    remote_runs = new HeapOrder(static_cast<uint16_t>(External_ids::Runs),
                                &runs_callback, &run_id);

    svpwm =
        new HeapOrder(static_cast<uint16_t>(Orders_id::SVPWM), &svpwm_callback,
                      &modulation_frequency_1, &commutation_frequency_1,
                      &reference_voltage_1, &max_voltage_1, &motor_direction_1);

    remote_svpwm = new HeapOrder(static_cast<uint16_t>(External_ids::SVPWM),
                                 &svpwm_callback, &modulation_frequency_1,
                                 &commutation_frequency_1, &reference_voltage_1,
                                 &max_voltage_1, &motor_direction_1);

    stop_motor = new HeapOrder(static_cast<uint16_t>(Orders_id::Stop_motor),
                               &stop_motor_callback);
    remote_stop_motor = new HeapOrder(
        static_cast<uint16_t>(External_ids::Stop_motor), &stop_motor_callback);

    current_control =
        new HeapOrder(static_cast<uint16_t>(Orders_id::Current_control),
                      &current_control_callback, &modulation_frequency_2,
                      &commutation_frequency_2, &reference_current_2,
                      &max_voltage_2, &motor_direction_2);

    remote_current_control =
        new HeapOrder(static_cast<uint16_t>(External_ids::Current_control),
                      &current_control_callback, &modulation_frequency_2,
                      &commutation_frequency_2, &reference_current_2,
                      &max_voltage_2, &motor_direction_2);

    speed_control = new HeapOrder(
        static_cast<uint16_t>(Orders_id::Speed_control),
        &speed_control_callback, &reference_speed_3, &commutation_frequency_3,
        &max_voltage_3, &motor_direction_3);

    remote_speed_control = new HeapOrder(
        static_cast<uint16_t>(External_ids::Speed_control),
        &speed_control_callback, &reference_speed_3, &commutation_frequency_3,
        &max_voltage_3, &motor_direction_3);

    motor_brake = new HeapOrder(static_cast<uint16_t>(Orders_id::Motor_brake),
                                &motor_brake_callback);

    remote_motor_brake =
        new HeapOrder(static_cast<uint16_t>(External_ids::Motor_brake),
                      &motor_brake_callback);

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
    control_station_udp->send_packet(*sdc);
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
    if (close_contactors_flag) {
        if (*VCU::operational_state == VCU_SM::OperationalStates::Idle) {
            static uint8_t timeout_id = -1;
            if (!close_contactors_sent) {
                hvscu_tcp->send_order(*remote_close_contactors);
                close_contactors_sent = true;
            } else {
                if (hvscu_state ==
                    static_cast<uint8_t>(HVSCU_states::HVSCU_Closed)) {
                    Time::cancel_timeout(timeout_id);
                    timeout_id = -1;
                    actuators->contactors_closed = true;
                    close_contactors_flag = false;
                    close_contactors_sent = false;
                } else {
                    if (timeout_id == -1) {
                        timeout_id = Time::set_timeout(6000, [&]() {
                            ProtectionManager::fault_and_propagate();
                        });
                    }
                }
            }
        } else {
            WARNING("Cannot close contactors in this state");
            close_contactors_flag = false;
        }
    }
}

void Comms::check_open_contactors_order() {
    if (open_contactors_flag) {
        static uint8_t timeout_id = -1;
        if (!open_contactors_sent) {
            hvscu_tcp->send_order(*remote_open_contactors);
            open_contactors_sent = true;
        } else {
            if (hvscu_state ==
                static_cast<uint8_t>(HVSCU_states::HVSCU_Opened)) {
                Time::cancel_timeout(timeout_id);
                timeout_id = -1;
                actuators->contactors_closed = false;
                open_contactors_flag = false;
                brakes->brake();  // Ensure brakes are applied when contactors
                                  // are open
                open_contactors_sent = false;
            } else {
                if (timeout_id == -1) {
                    timeout_id = Time::set_timeout(100, [&]() {
                        ProtectionManager::fault_and_propagate();
                    });
                }
            }
        }
    }
}

void Comms::check_brake_order() {
    if (brake_flag) {
        if (*VCU::operational_state == VCU_SM::OperationalStates::Ready ||
            *VCU::operational_state ==
                VCU_SM::OperationalStates::Demonstration) {
            brakes->brake();
            actuators->set_regulator_1(0);
            brake_flag = false;
        } else {
            WARNING("Cannot brake in this state");
            brake_flag = false;
        }
    }
}

void Comms::check_unbrake_order() {
    if (unbrake_flag) {
        if (*VCU::operational_state == VCU_SM::OperationalStates::Energized) {
            brakes->unbrake();
            unbrake_flag = false;
        } else {
            WARNING("Cannot unbrake in this state");
            unbrake_flag = false;
        }
    }
}

void Comms::check_propulsion_order() {
    if (propulsion_flag) {
        if (*VCU::operational_state == VCU_SM::OperationalStates::Ready ||
            *VCU::operational_state ==
                VCU_SM::OperationalStates::Demonstration) {
            static uint8_t timeout_id = -1;

            if (!propulsion_sent) {
                pcu_tcp->send_order(*remote_levitation);
                propulsion_sent = true;
            } else {
                if (pcu_state ==
                    static_cast<uint8_t>(PCU_states::PCU_Propulsion)) {
                    Time::cancel_timeout(timeout_id);
                    timeout_id = -1;
                    order_demonstration_bitfield |= (1UL << propulsion_bit);
                    propulsion_flag = false;
                    propulsion_sent = false;
                } else {
                    if (timeout_id == -1) {
                        timeout_id = Time::set_timeout(100, [&]() {
                            ProtectionManager::fault_and_propagate();
                        });
                    }
                }
            }
        } else {
            WARNING("Cannot propel in this state");
            levitation_flag = false;
        }
    }
}

void Comms::check_stop_propulsion_order() {
    if (stop_levitation_flag) {
        if (*VCU::operational_state ==
            VCU_SM::OperationalStates::Demonstration) {
            static uint8_t timeout_id = -1;

            if (!stop_levitation_sent) {
                lcu_tcp->send_order(*remote_stop_levitation);
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
                            ProtectionManager::fault_and_propagate();
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

//--------- PCU ----------------------------------------------------------

void Comms::check_runs_order() {
    if (runs_flag) {
        if (*VCU::operational_state == VCU_SM::OperationalStates::Ready ||
            *VCU::operational_state ==
                VCU_SM::OperationalStates::Demonstration) {
            static uint8_t timeout_id = -1;

            if (!runs_sent) {
                pcu_tcp->send_order(*remote_runs);
                runs_sent = true;
            } else {
                if (pcu_state ==
                    static_cast<uint8_t>(PCU_states::PCU_Propulsion)) {
                    Time::cancel_timeout(timeout_id);
                    timeout_id = -1;
                    order_demonstration_bitfield |= (1UL << propulsion_bit);
                    runs_flag = false;
                    runs_sent = false;
                } else {
                    if (timeout_id == -1) {
                        timeout_id = Time::set_timeout(100, [&]() {
                            ProtectionManager::fault_and_propagate();
                        });
                    }
                }
            }
        } else {
            WARNING("Cannot start a run in this state");
            runs_flag = false;
        }
    }
}

void Comms::check_svpwm_order() {
    if (svpwm_flag) {
        if (*VCU::operational_state == VCU_SM::OperationalStates::Ready ||
            *VCU::operational_state ==
                VCU_SM::OperationalStates::Demonstration) {
            static uint8_t timeout_id = -1;

            if (!svpwm_sent) {
                pcu_tcp->send_order(*remote_svpwm);
                svpwm_sent = true;
            } else {
                if (pcu_state ==
                    static_cast<uint8_t>(PCU_states::PCU_Propulsion)) {
                    Time::cancel_timeout(timeout_id);
                    timeout_id = -1;
                    order_demonstration_bitfield |= (1UL << propulsion_bit);
                    svpwm_flag = false;
                    svpwm_sent = false;
                } else {
                    if (timeout_id == -1) {
                        timeout_id = Time::set_timeout(100, [&]() {
                            ProtectionManager::fault_and_propagate();
                        });
                    }
                }
            }
        } else {
            WARNING("Cannot send SVPWM in this state");
            svpwm_flag = false;
        }
    }
}

void Comms::check_stop_motor_order() {
    if (stop_motor_flag) {
        if (*VCU::operational_state ==
            VCU_SM::OperationalStates::Demonstration) {
            static uint8_t timeout_id = -1;

            if (!stop_motor_sent) {
                pcu_tcp->send_order(*remote_stop_motor);
                stop_motor_sent = true;
            } else {
                if (pcu_state ==
                    static_cast<uint8_t>(PCU_states::PCU_Stop_Propulsion)) {
                    Time::cancel_timeout(timeout_id);
                    timeout_id = -1;
                    order_demonstration_bitfield &= ~(1UL << propulsion_bit);
                    stop_motor_flag = false;
                    stop_motor_sent = false;
                } else {
                    if (timeout_id == -1) {
                        timeout_id = Time::set_timeout(100, [&]() {
                            ProtectionManager::fault_and_propagate();
                        });
                    }
                }
            }
        } else {
            WARNING("Cannot stop the motor in this state");
            stop_motor_flag = false;
        }
    }
}

void Comms::check_current_control_order() {
    if (current_control_flag) {
        if (*VCU::operational_state == VCU_SM::OperationalStates::Ready ||
            *VCU::operational_state ==
                VCU_SM::OperationalStates::Demonstration) {
            static uint8_t timeout_id = -1;

            if (!current_control_sent) {
                pcu_tcp->send_order(*remote_current_control);
                current_control_sent = true;
            } else {
                if (pcu_state ==
                    static_cast<uint8_t>(PCU_states::PCU_Propulsion)) {
                    Time::cancel_timeout(timeout_id);
                    timeout_id = -1;
                    order_demonstration_bitfield |= (1UL << propulsion_bit);
                    current_control_flag = false;
                    current_control_sent = false;
                } else {
                    if (timeout_id == -1) {
                        timeout_id = Time::set_timeout(100, [&]() {
                            ProtectionManager::fault_and_propagate();
                        });
                    }
                }
            }
        } else {
            WARNING("Cannot send current control in this state");
            current_control_flag = false;
        }
    }
}

void Comms::check_speed_control_order() {
    if (speed_control_flag) {
        if (*VCU::operational_state == VCU_SM::OperationalStates::Ready ||
            *VCU::operational_state ==
                VCU_SM::OperationalStates::Demonstration) {
            static uint8_t timeout_id = -1;

            if (!speed_control_sent) {
                pcu_tcp->send_order(*remote_speed_control);
                speed_control_sent = true;
            } else {
                if (pcu_state ==
                    static_cast<uint8_t>(PCU_states::PCU_Propulsion)) {
                    Time::cancel_timeout(timeout_id);
                    timeout_id = -1;
                    order_demonstration_bitfield |= (1UL << propulsion_bit);
                    speed_control_flag = false;
                    speed_control_sent = false;
                } else {
                    if (timeout_id == -1) {
                        timeout_id = Time::set_timeout(100, [&]() {
                            ProtectionManager::fault_and_propagate();
                        });
                    }
                }
            }
        } else {
            WARNING("Cannot send speed control in this state");
            speed_control_flag = false;
        }
    }
}

void Comms::check_motor_brake_order() {
    if (motor_brake_flag) {
        if (*VCU::operational_state == VCU_SM::OperationalStates::Ready ||
            *VCU::operational_state ==
                VCU_SM::OperationalStates::Demonstration) {
            static uint8_t timeout_id = -1;

            if (!motor_brake_sent) {
                pcu_tcp->send_order(*remote_motor_brake);
                motor_brake_sent = true;
            } else {
                if (pcu_state ==
                    static_cast<uint8_t>(PCU_states::PCU_Propulsion)) {
                    Time::cancel_timeout(timeout_id);
                    timeout_id = -1;
                    order_demonstration_bitfield |= (1UL << propulsion_bit);
                    motor_brake_flag = false;
                    motor_brake_sent = false;
                } else {
                    if (timeout_id == -1) {
                        timeout_id = Time::set_timeout(100, [&]() {
                            ProtectionManager::fault_and_propagate();
                        });
                    }
                }
            }
        } else {
            WARNING("Cannot brake with the motor in this state");
            motor_brake_flag = false;
        }
    }
}

//--------- PCU ----------------------------------------------------------

void Comms::check_levitation_order() {
    if (levitation_flag) {
        if (*VCU::operational_state == VCU_SM::OperationalStates::Ready ||
            *VCU::operational_state ==
                VCU_SM::OperationalStates::Demonstration) {
            static uint8_t timeout_id = -1;

            if (!levitation_sent) {
                lcu_tcp->send_order(*remote_levitation);
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
                            ProtectionManager::fault_and_propagate();
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
                lcu_tcp->send_order(*remote_stop_levitation);
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
                            ProtectionManager::fault_and_propagate();
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
                lcu_tcp->send_order(*remote_booster);
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
                            ProtectionManager::fault_and_propagate();
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
                lcu_tcp->send_order(*remote_stop_booster);
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
                            ProtectionManager::fault_and_propagate();
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
    check_open_contactors_order();

    check_brake_order();
    check_unbrake_order();

    check_levitation_order();
    check_stop_levitation_order();

    check_booster_order();
    check_stop_booster_order();

    check_runs_order();
    check_svpwm_order();
    check_stop_motor_order();
    check_current_control_order();
    check_speed_control_order();
    check_motor_brake_order();
}

void Comms::update() { check_orders(); }