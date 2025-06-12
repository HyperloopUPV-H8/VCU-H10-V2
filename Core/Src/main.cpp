#ifndef SIM_ON
#include "main.h"

#include "lwip.h"
#endif

#include "ST-LIB.hpp"

enum Pump{
    PUMP_UNIDADES = 0,
    PUMP_PLACAS = 1
};

enum Regulator{
    REGULATOR_1 = 0,
    REGULATOR_2 = 1
};



//Potencia refri
uint32_t potencia_unidades = 0;
Pump selected_pump;

PWM Pump_c1;
PWM Pump_c2;


//Regulator out
float Selected_Regulator_pressure=0;//me falta 1 por hacer , los 2 el mismo??
Regulator selected_regulator;

PWM Regulator_out1;//por caracterizar
PWM Regulator_out2;

//Actuator:
bool Activate_actuator = false;
DigitalOutput Actuator_out;

static void Pump_callback() {// hay que caracterizar el pwm en estas 2
    if(selected_pump == Pump::PUMP_UNIDADES) {
        Pump_c1.set_duty_cycle(potencia_unidades);
    } else if (selected_pump == Pump::PUMP_PLACAS) {
        Pump_c2.set_duty_cycle(potencia_unidades);
    }
}

static void Activate_actuator_callback() {
    if(Activate_actuator) {
        Actuator_out.turn_on();
    } else {
        Actuator_out.turn_off();
    }
}

static void Set_regulator_callback() {
    float regulator_value = static_cast<float>(Selected_Regulator_pressure * (100.0 / 0.9));
    if(selected_regulator == Regulator::REGULATOR_1){
        Regulator_out1.set_duty_cycle(regulator_value);
    }else if(selected_regulator == Regulator::REGULATOR_2){
        Regulator_out2.set_duty_cycle(regulator_value);
    }    
}



int main(void) {
#ifdef SIM_ON
    SharedMemory::start();
#endif

    Actuator_out = DigitalOutput(PE7);
    Pump_c1=PWM(PE9);
    Pump_c2=PWM(PE11);

    Regulator_out1=PWM(PB9);//por caracterizar
    Regulator_out2=PWM(PD13);

    //SDC_sense
    PinState Sdc;
    DigitalSensor SDC_sense(PA12,&Sdc); 

    //reeds
    PinState reed1;
    PinState reed2;
    PinState reed3;
    PinState reed4;
    PinState reed5;
    PinState reed6;
    PinState reed7;
    PinState reed8;

    DigitalSensor reed1_input(PD11,&reed1);
    DigitalSensor reed2_input(PD10,&reed2);
    DigitalSensor reed3_input(PG4,&reed3);
    DigitalSensor reed4_input(PG3,&reed4);
    DigitalSensor reed5_input(PD8,&reed5);
    DigitalSensor reed6_input(PD9,&reed6);
    DigitalSensor reed7_input(PB15,&reed7);
    DigitalSensor reed8_input(PB14,&reed8);

    
    

    
   
    //in:
    PinState regulator_1;
    PinState regulator_2;
    DigitalSensor Regulator_in1(PA4,&regulator_1);
    DigitalSensor Regulator_in2(PD12,&regulator_2);
    // DigitalInput Regulator_in2;
    // Regulator_in2.inscribe(PD12);

    //Tapes(temporal):
    DigitalOutput TapeE(PG1);
    

    //flow:

    PinState flow1;
    PinState flow2;


    DigitalSensor flow1_input(PE13,&flow1);
    DigitalSensor flow2_input(PE14,&flow2);
    
    float pressure_1 = 0;
    float pressure_2 = 0;
    float pressure_3 = 0;
    float pressure_4 = 0;

    LinearSensor <float> PresionAlta(PB0,1,0,&pressure_1);
    LinearSensor <float> PresionRegulador(PB1,1,0,&pressure_2);
    LinearSensor <float> PresionFrenos(PF11,1,0,&pressure_3);
    LinearSensor <float> PresionCapsula(PF12,1,0,&pressure_4);


    HeapOrder Potencia_Pump{ static_cast<uint16_t>(1731),&Pump_callback,&potencia_unidades,&selected_pump};
    HeapOrder Activate_actuator_order( static_cast<uint16_t>(8456),&Activate_actuator_callback,&Activate_actuator);
    HeapOrder Set_regulator( static_cast<uint16_t>(9576),&Set_regulator_callback,&Selected_Regulator_pressure,&selected_regulator);

    static const IPV4 VCU_IP{"192.168.1.3"};
    static const IPV4 control_station_ip{"192.168.0.9"};
    static const uint16_t local_port{50500};
    static const uint16_t controlstation_port{50555};
    static const uint16_t udp_controlstation_port{50400};

    HeapPacket Reeds{3124,&reed1,&reed2,&reed3,&reed4,&reed5,&reed6,&reed7,&reed8};
    HeapPacket flow(6457,&flow1,&flow2);
    HeapPacket Regulator(1312,&regulator_1);
    HeapPacket Pression(1312,&pressure_1,&pressure_2,&pressure_3,&pressure_4);

    string mask,gateway,ip;
    mask ="255.255.0.0";
    gateway = "192.168.1.1";
    ip = "192.168.1.3";


    STLIB::start("00:80:e1:00:00:00",ip, mask, gateway, UART::uart2);

    ServerSocket Control_station(VCU_IP,local_port);
    DatagramSocket udp_controlstation(VCU_IP,udp_controlstation_port,control_station_ip,controlstation_port);

    Pump_c1.set_frequency(10000);
    Pump_c1.set_duty_cycle(0); 
    Pump_c1.turn_on();

    Pump_c2.set_frequency(10000);
    Pump_c2.set_duty_cycle(0); 
    Pump_c2.turn_on();

    Regulator_out1.set_frequency(30000);
    Regulator_out1.set_duty_cycle(0);
    Regulator_out1.turn_on();

    Regulator_out2.set_frequency(30000);
    Regulator_out2.set_duty_cycle(0);
    Regulator_out2.turn_on();

    TapeE.turn_on();//Hardcoded for now, es logica inversa okay?

    Time::register_low_precision_alarm(16, [&]() {
        // SDC_sense.read();
        
        // reed1_input.read();
        // reed2_input.read();
        // reed3_input.read();
        // reed4_input.read();
        // reed5_input.read();
        // reed6_input.read();
        // reed2_input.read();
        // reed3_input.read();
        // reed4_input.read();
        // reed5_input.read();
        // reed6_input.read();
        // reed7_input.read();
        // reed8_input.read();
        // reed7_input.read();
        // reed8_input.read();

        Regulator_in1.read();
        Regulator_in2.read();

        flow1_input.read();
        flow2_input.read();

        PresionAlta.read();
        PresionRegulador.read();
        PresionFrenos.read();
        PresionCapsula.read();

        // udp_controlstation.send_packet(Reeds);
        // udp_controlstation.send_packet(flow);
        udp_controlstation.send_packet(Regulator);
        udp_controlstation.send_packet(Pression);

    });

    while (1) {
        STLIB::update();
    }
}

void Error_Handler(void) {
    ErrorHandler("HAL error handler triggered");
    while (1) {
    }
}
