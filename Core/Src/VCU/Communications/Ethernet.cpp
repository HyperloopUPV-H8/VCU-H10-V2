#pragma once
#include "Ethernet.hpp"

namespace Communications {
Ethernet::Ethernet() {
    Socket = new ServerSocket(local_ip,tcp_server_port);
    packets_endpoint = new DatagramSocket(local_ip, udp_port,control_station_ip, udp_port);
    
}
};