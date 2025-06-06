#ifndef SIM_ON
#include "main.h"

#include "lwip.h"
#endif

#include "ST-LIB.hpp"

PWM Pump_c1(PE9);
PWM Pump_c2(PE11);

PWM Regulator_out1(PB9);//por caracterizar
PWM Regulator_out2(PD13);

//Actuator:
bool Activate_actuator = false;
DigitalOutput Actuator_out(PG1);

static void Potencia_unidades_callback() {// hay que caracterizar el pwm en estas 2
    //Lo que haga el callback
}

static void Potencia_placas_callback() {
    //Lo que haga el callback
}

static void Activate_actuator_callback() {
    if(Activate_actuator) {
        Actuator_out.set_pin_state(PinState::ON);
    } else {
        Actuator_out.set_pin_state(PinState::OFF);
    }
}

static void Set_regulator_callback() {
    //Pasar a frecuencia y duty los porcentajes:
    
}



int main(void) {
#ifdef SIM_ON
    SharedMemory::start();
#endif
    //SDC_sense
    DigitalInput SDC_sense;
    SDC_sense.inscribe(PA12); 

    //reeds
    DigitalInput reed1_input;
    DigitalInput reed2_input;
    DigitalInput reed3_input;
    DigitalInput reed4_input;
    DigitalInput reed5_input;
    DigitalInput reed6_input;
    DigitalInput reed7_input;
    DigitalInput reed8_input;

    reed1_input.inscribe(PD11);
    reed2_input.inscribe(PD10);
    reed3_input.inscribe(PG4);
    reed4_input.inscribe(PG3);
    reed5_input.inscribe(PD8);
    reed6_input.inscribe(PD9);
    reed7_input.inscribe(PB15);
    reed8_input.inscribe(PB14);

    uint32_t reed1;
    uint32_t reed2;
    uint32_t reed3;
    uint32_t reed4;
    uint32_t reed5;
    uint32_t reed6;
    uint32_t reed7;

    //Potencia refri
    uint32_t potencia_unidades = 0;
    uint32_t potencia_placas = 0;//Pasarlo a float?

    

    //Regulator out
    uint32_t Selected_Regulator_pressure=0;//me falta 1 por hacer 
   
    //in:
    uint32_t regulator_1;
    // uint32_t regulator_2;
    DigitalInput Regulator_in1;
    Regulator_in1.inscribe(PA4);
    DigitalInput Regulator_in2;
    Regulator_in2.inscribe(PD12);


    

    //flow:
    DigitalInput flow1_input;
    DigitalInput flow2_input;

    flow1_input.inscribe(PE13);
    flow2_input.inscribe(PE14);
    uint32_t flow1;
    uint32_t flow2;

    

    



    HeapOrder Potencia_Pump_Unidades{ static_cast<uint16_t>(173145),&Potencia_unidades_callback,&potencia_unidades};
    HeapOrder Potencia_Pump_Placas{ static_cast<uint16_t>(17491),&Potencia_placas_callback,&potencia_placas};
    HeapOrder Activate_actuator_order( static_cast<uint16_t>(84564),&Activate_actuator_callback,&Activate_actuator);
    HeapOrder Set_regulator( static_cast<uint16_t>(957654),&Set_regulator_callback,&Selected_Regulator_pressure);

    static const IPV4 VCU_IP{"192.168.1.3"};
    static const IPV4 control_station_ip{"192.168.0.9"};
    static const MAC local_mac{"00:80:E1:11:02:00"};
    static const uint16_t local_port{50500};//Habra que ver tambien los puertos
    static const uint16_t controlstation_port{50555};
    static const uint16_t udp_controlstation_port{50400};

    ServerSocket Control_station(VCU_IP,local_port);
    DatagramSocket udp_controlstation(VCU_IP,udp_controlstation_port,control_station_ip,controlstation_port);

    HeapPacket Reeds{31241,&reed1,&reed2,&reed3,&reed4,&reed5,&reed6,&reed7};
    HeapPacket flow(64578,&flow1,&flow2);
    HeapPacket Regulator(13124,&regulator_1);



    STLIB::start();


    while (1) {
        STLIB::update();
    }
}

void Error_Handler(void) {
    ErrorHandler("HAL error handler triggered");
    while (1) {
    }
}
