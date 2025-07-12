#pragma once
#include "ST-LIB.hpp"
#include "VCU/Comms/Comms.hpp"

using namespace std::chrono_literals;

class VCU_SM {
   public:
    enum GeneralStates {
        Connecting,
        Operational,
        Fault,
    };

    enum OperationalStates { Idle, EndOfRun, Energized, Ready, Demonstration };

    StateMachine GeneralStateMachine;
    StateMachine OperationalStateMachine;

    VCU_SM() {
        GeneralStateMachine = StateMachine(GeneralStates::Connecting);
        GeneralStateMachine.add_state(GeneralStates::Operational);
        GeneralStateMachine.add_state(GeneralStates::Fault);

        OperationalStateMachine = StateMachine(OperationalStates::Idle);
        OperationalStateMachine.add_state(OperationalStates::EndOfRun);
        OperationalStateMachine.add_state(OperationalStates::Energized);
        OperationalStateMachine.add_state(OperationalStates::Ready);
        OperationalStateMachine.add_state(OperationalStates::Demonstration);

        GeneralStateMachine.add_state_machine(OperationalStateMachine,
                                              GeneralStates::Operational);

        GeneralStateMachine.add_transition(
            GeneralStates::Connecting, GeneralStates::Operational, [&]() {
                return Comms::control_station_tcp->is_connected() &&
                       Comms::hvscu_tcp->is_connected() &&
                       Comms::pcu_tcp->is_connected() &&
                       Comms::lcu_tcp->is_connected() &&
                       Comms::bcu_tcp->is_connected();
            });

        GeneralStateMachine.add_transition(
            GeneralStates::Operational, GeneralStates::Fault, [&]() {
                return !Comms::control_station_tcp->is_connected() ||
                       !Comms::hvscu_tcp->is_connected() ||
                       !Comms::pcu_tcp->is_connected() ||
                       !Comms::lcu_tcp->is_connected() ||
                       !Comms::bcu_tcp->is_connected();
            });

        GeneralStateMachine.add_transition(
            GeneralStates::Operational, GeneralStates::Fault, [&]() {
                return ((Comms::brakes->All_reeds &&
                         Comms::brakes->Active_brakes) &&
                        (!Comms::brakes->breaks_first_time));
            });

        /* GeneralStateMachine.add_transition(
            GeneralStates::Connecting, GeneralStates::Fault, [&]() {
                return ((Comms::brakes->All_reeds &&
                         Comms::brakes->Active_brakes) &&
                        (!Comms::brakes->breaks_first_time));
            }); */

        GeneralStateMachine.add_transition(
            GeneralStates::Operational, GeneralStates::Fault,
            [&]() { return (!Comms::actuators->Sdc); });

        /* GeneralStateMachine.add_transition(
            GeneralStates::Connecting, GeneralStates::Fault,
            [&]() { return (!Comms::actuators->Sdc); }); */

        GeneralStateMachine.add_transition(
            GeneralStates::Operational, GeneralStates::Fault, [&]() {
                bool emergency_tape =
                    static_cast<bool>(Comms::brakes->tape_emergency);
                return !emergency_tape &&
                       Comms::brakes->tape_enable_status == PinState::ON;
            });

        GeneralStateMachine.add_enter_action(
            [&]() { Comms::leds->leds_connecting(); },
            GeneralStates::Connecting);

        GeneralStateMachine.add_enter_action(
            [&]() { Comms::leds->leds_operational(); },
            GeneralStates::Operational);

        GeneralStateMachine.add_enter_action(
            [&]() {
                Comms::leds->leds_fault();

                Time::set_timeout(100, [&]() { Comms::brakes->brake(); });
            },
            GeneralStates::Fault);

        //-----------

        OperationalStateMachine.add_transition(
            OperationalStates::Idle, OperationalStates::Energized,
            [&]() { return Comms::actuators->contactors_closed; });

        OperationalStateMachine.add_transition(
            OperationalStates::Energized, OperationalStates::Idle,
            [&]() { return !Comms::actuators->contactors_closed; });

        OperationalStateMachine.add_transition(
            OperationalStates::Demonstration, OperationalStates::Idle,
            [&]() { return !Comms::actuators->contactors_closed; });

        OperationalStateMachine.add_transition(
            OperationalStates::Ready, OperationalStates::Idle,
            [&]() { return !Comms::actuators->contactors_closed; });

        OperationalStateMachine.add_transition(
            OperationalStates::Energized, OperationalStates::Ready,
            [&]() { return !Comms::brakes->Active_brakes; });

        OperationalStateMachine.add_transition(
            OperationalStates::Ready, OperationalStates::Energized,
            [&]() { return Comms::brakes->Active_brakes; });

        //-----------

        OperationalStateMachine.add_transition(
            OperationalStates::Ready, OperationalStates::Demonstration,
            [&]() { return Comms::order_demonstration_bitfield > 0; });

        OperationalStateMachine.add_transition(
            OperationalStates::Demonstration, OperationalStates::Ready,
            [&]() { return Comms::order_demonstration_bitfield == 0; });

        //-----------------CYCLYC ACTIONS-----------------------

        GeneralStateMachine.add_low_precision_cyclic_action(
            [&]() { Comms::packet_sending = true; }, 100ms,
            GeneralStates::Connecting);
        GeneralStateMachine.add_low_precision_cyclic_action(
            [&]() { Comms::packet_sending = true; }, 100ms,
            GeneralStates::Operational);
        GeneralStateMachine.add_low_precision_cyclic_action(
            [&]() { Comms::packet_sending = true; }, 100ms,
            GeneralStates::Fault);

        GeneralStateMachine.add_low_precision_cyclic_action(
            [&]() { Comms::reading_sensors = true; }, 100ms,
            GeneralStates::Connecting);
        GeneralStateMachine.add_low_precision_cyclic_action(
            [&]() { Comms::reading_sensors = true; }, 100ms,
            GeneralStates::Operational);
        GeneralStateMachine.add_low_precision_cyclic_action(
            [&]() { Comms::reading_sensors = true; }, 100ms,
            GeneralStates::Fault);

        GeneralStateMachine.add_low_precision_cyclic_action(
            [&]() {
                if (!Comms::hvscu_tcp->is_connected()) {
                    Comms::hvscu_tcp->reconnect();
                }

                if (!Comms::pcu_tcp->is_connected()) {
                    Comms::pcu_tcp->reconnect();
                }

                if (!Comms::lcu_tcp->is_connected()) {
                    Comms::lcu_tcp->reconnect();
                }

                if (!Comms::bcu_tcp->is_connected()) {
                    Comms::bcu_tcp->reconnect();
                }
            },
            100ms, GeneralStates::Connecting);
    }
};