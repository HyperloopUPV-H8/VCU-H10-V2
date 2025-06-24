#include "VCU/Actuators/Pinout.hpp"

namespace Pinout {
Pin& led_operational_pin(LED_OPERATIONAL_PIN);
Pin& led_fault_pin(LED_FAULT_PIN);
Pin& led_can_pin(LED_CAN_PIN);
Pin& led_sleep_pin(LED_SLEEP_PIN);
Pin& led_flash_pin(LED_FLASH_PIN);

Pin& reed1_pin(REED1_PIN);
Pin& reed2_pin(REED2_PIN);
Pin& reed3_pin(REED3_PIN);
Pin& reed4_pin(REED4_PIN);
Pin& reed5_pin(REED5_PIN);
Pin& reed6_pin(REED6_PIN);
Pin& reed7_pin(REED7_PIN);
Pin& reed8_pin(REED8_PIN);

Pin& regulator_1_in_pin(REGULATOR_1_IN_PIN);
Pin& regulator_2_in_pin(REGULATOR_2_IN_PIN);
Pin& regulator_1_out_pin(REGULATOR_1_OUT_PIN);
Pin& regulator_2_out_pin(REGULATOR_2_OUT_PIN);

Pin& Brake_pin(BRAKE_PIN);
Pin& SDC_pin(SDC_PIN);
Pin& Pump_c1_pin(PUMP_C1_PIN);
Pin& Pump_c2_pin(PUMP_C2_PIN);
Pin& TapeE_pin(TAPE_E_PIN);
Pin& Flow1_pin(FLOW1_PIN);
Pin& Flow2_pin(FLOW2_PIN);
Pin& Tape_pin(TAPE_PIN);

Pin& Presion_alta_pin(PRESION_ALTA_PIN);
Pin& Presion_regulador_pin(PRESION_REGULADOR_PIN);
Pin& Presion_frenos_pin(PRESION_FRENOS_PIN);
Pin& Presion_capsula_pin(PRESION_CAPSULA_PIN);
}