#include "VCU/Comms.hpp"

void Comms::init(){
    control_station_tcp = new ServerSocket(IPV4(VCU_IP), LOCAL_PORT);
    control_station_udp = new DatagramSocket(IPV4(VCU_IP), CONTROL_STATION_UDP_PORT,IPV4(CONTROL_SATION_IP), CONTROL_STATION_UDP_PORT);

    //add_packets();
    //add_orders();
}

void Comms::add_packets(){

}

void Comms::add_orders(){

}