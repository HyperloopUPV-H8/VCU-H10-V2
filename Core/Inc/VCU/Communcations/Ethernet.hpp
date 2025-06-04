#pragma once
#include "ST-LIB.hpp"

//Esto luego será el ethernet:
namespace Communications{
class Ethernet{

public:

//TENGO QUE METER LOS PAQUETES AQUI, O RECIBIR LOS PARAMETROS POR REFERENCIA EN EL CONSTRUCTOR


inline static const IPV4 local_ip{"192.168.1.3"}; //Habra que ver las ips
inline static const IPV4 control_station_ip{"192.168.0.9"};

inline static const MAC local_mac{"00:80:E1:11:02:00"};

inline static const uint16_t tcp_server_port{50500};//Habra que ver tambien los puertos
inline static const uint16_t tcp_client_port{50401};
inline static const uint16_t udp_port{50400};


static ServerSocket* Socket = nullptr;
static DatagramSocket* packets_endpoint = nullptr;
static std::vector<HeapPacket*> packets{};
Ethernet();


private:



//definir todo el ethernet


}:
}: // namespace VCU::Ethernet

