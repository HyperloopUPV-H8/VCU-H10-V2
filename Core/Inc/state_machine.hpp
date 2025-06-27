#pragma once
#include "ST-LIB.hpp"
#include "VCU/Actuators.hpp"
#include "VCU/Brakes.hpp"
#include "VCU/Comms.hpp"
#include "VCU/Leds.hpp"

using namespace std::chrono_literals;

class VCU_SM {
   public:
    enum GeneralStates {
        Connecting,
        Operational,
        Fault,
    };

    enum OperationalStates {
        Idle,
        EndOfRun,
        Energyzed,
        Ready,
    };

    StateMachine GeneralStateMachine;
    StateMachine OperationalStateMachine;

    Leds leds;
    Actuators actuators;
    Brakes brakes;

    VCU_SM() {
        GeneralStateMachine = StateMachine(GeneralStates::Connecting);
        GeneralStateMachine.add_state(GeneralStates::Operational);
        GeneralStateMachine.add_state(GeneralStates::Fault);

        OperationalStateMachine = StateMachine(OperationalStates::Idle);
        OperationalStateMachine.add_state(OperationalStates::EndOfRun);
        OperationalStateMachine.add_state(OperationalStates::Energyzed);
        OperationalStateMachine.add_state(OperationalStates::Ready);

        GeneralStateMachine.add_transition(
            GeneralStates::Connecting, GeneralStates::Operational,
            [&]() { return Comms::control_station_tcp->is_connected(); });

        GeneralStateMachine.add_transition(
            GeneralStates::Operational, GeneralStates::Fault,
            [&]() { return !Comms::control_station_tcp->is_connected(); });

        GeneralStateMachine.add_transition(
            GeneralStates::Operational, GeneralStates::Fault, [&]() {
                return ((brakes.All_reeds && brakes.Active_brakes) &&
                        (!brakes.breaks_first_time));
            });

        GeneralStateMachine.add_transition(
            GeneralStates::Connecting, GeneralStates::Fault, [&]() {
                return ((brakes.All_reeds && brakes.Active_brakes) &&
                        (!brakes.breaks_first_time));
            });

        GeneralStateMachine.add_transition(GeneralStates::Operational,
                                           GeneralStates::Fault,
                                           [&]() { return (!actuators.Sdc); });

        GeneralStateMachine.add_transition(GeneralStates::Connecting,
                                           GeneralStates::Fault,
                                           [&]() { return (!actuators.Sdc); });

        GeneralStateMachine.add_enter_action([&]() { leds.leds_connecting(); },
                                             GeneralStates::Connecting);

        GeneralStateMachine.add_enter_action([&]() { leds.leds_operational(); },
                                             GeneralStates::Operational);

        GeneralStateMachine.add_enter_action(
            [&]() {
                leds.leds_fault();
                HAL_Delay(100);
                brakes.brake();
            },
            GeneralStates::Fault);

        // el sdc tambien manda a fault?

        /* OperationalStateMachine.add_transition(
            OperationalStates::Idle, OperationalStates::Energyzed,
            [&]() { return ethernet->requested_close_contactors; });

        OperationalStateMachine.add_transition(
            OperationalStates::Idle, OperationalStates::EndOfRun,
            [&]() { return ethernet->requested_end_of_run; });

        OperationalStateMachine.add_transition(
            OperationalStates::Energyzed, OperationalStates::Idle,
            [&]() { return ethernet->requested_open_contactors; }); */

        OperationalStateMachine.add_transition(
            OperationalStates::Ready, OperationalStates::Energyzed,
            [&]() { return brakes.Active_brakes; });

        OperationalStateMachine.add_transition(
            OperationalStates::Energyzed, OperationalStates::Ready,
            [&]() { return (!brakes.Active_brakes); });

        /* OperationalStateMachine.add_enter_action(
            [&]() { ethernet->requested_close_contactors = false; },
            OperationalStates::Energyzed);

        OperationalStateMachine.add_enter_action(
            [&]() { ethernet->requested_end_of_run = false; },
            OperationalStates::EndOfRun);

        OperationalStateMachine.add_enter_action(
            [&]() { ethernet->requested_open_contactors = false; },
            OperationalStates::Idle); */

        // ethernet->initialize_state_orders();
    }
};