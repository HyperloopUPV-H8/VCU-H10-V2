from vmcu.services.communications.Socket import Socket
from vmcu.services.communications.ServerSocket import Server
from vmcu.shared_memory import SharedMemory
from vmcu.services.digital_out import DigitalOutService
from vmcu.pin.pinout import Pinout
from vmcu.services.communications.Packets import Packets
from enum import IntEnum
from collections import namedtuple
from vmcu.pin.memory import DigitalOut
import struct
#Orders needed for the VCU
CLOSE_CONTACTORS = 900
OPEN_CONTACTORS = 901
BRAKE = 902
UNBRAKE = 903
START_VERTICAL_LEVITATION = 904
STOP_VERTICAL_LEVITATION = 905
START_HORIZONTAL_LEVITATION = 906
STOP_HORIZONTAL_LEVITATION = 907
START_PROPULSION = 908
STOP_PROPULSION = 909
START_CHARGING_LV = 910
STOP_CHARGING_LV = 911
START_CHARGING_G2V = 912
STOP_CHARGING_G2V = 913
ENABLES_BOOSTER = 914
DISABLES_BOOSTER = 915
END_OF_RUN = 916
STICK_DOWN = 917
ORDERS_VCU = [ CLOSE_CONTACTORS, OPEN_CONTACTORS, BRAKE, UNBRAKE ,
                START_VERTICAL_LEVITATION , STOP_VERTICAL_LEVITATION, START_HORIZONTAL_LEVITATION,
                STOP_HORIZONTAL_LEVITATION, START_PROPULSION, STOP_PROPULSION, START_CHARGING_LV, STOP_CHARGING_LV ,
                START_CHARGING_G2V, STOP_CHARGING_G2V, ENABLES_BOOSTER, DISABLES_BOOSTER, END_OF_RUN, STICK_DOWN]

ORDERS_WITH_ARGS = [START_PROPULSION, START_VERTICAL_LEVITATION]

ORDERS_IDLE = [ START_CHARGING_G2V, STOP_CHARGING_G2V , OPEN_CONTACTORS , CLOSE_CONTACTORS, END_OF_RUN]
ORDERS_Energized = [BRAKE, UNBRAKE]
ORDERS_READY = [START_VERTICAL_LEVITATION, STOP_VERTICAL_LEVITATION, START_HORIZONTAL_LEVITATION, STICK_DOWN,
                STOP_HORIZONTAL_LEVITATION, START_PROPULSION, STOP_PROPULSION , ENABLES_BOOSTER , DISABLES_BOOSTER]

#Boards connected
BOARD = namedtuple('Board', ['address', 'port'])
LCU = BOARD(address='192.168.1.3', port=5003)
PCU = BOARD(address='192.168.1.5', port=5005)
BMSL = BOARD(address='192.168.1.254',port=5254)
HVSCU = BOARD(address='192.168.1.7',port = 5007)
BLCU = BOARD(address='192.168.1.8',port=5008)
GUI = BOARD(address='192.168.0.9',port=5009)
VCU_address = BOARD(address='192.168.1.2',port = 5002)
        
BOARDS = [LCU,PCU,BMSL,HVSCU,BLCU]
class General_SM(IntEnum):
        CONNECTING = 0
        OPERATIONAL = 1
        FAULT = 2

class Operational_SM(IntEnum):
    IDLE = 0
    Energized = 1
    READY = 2
    CHARGING_G2V = 3
    ENDOFRUN = 4

class VCU:

    def __init__(self):
        self._shm = SharedMemory("shm_gpio_VCU", "shm_sm_VCU")
        self._gui = Socket(VCU_address.address,VCU_address.port, GUI.address, GUI.port)
        self._boards_server_sockets =  [Server(board.address, board.port) for board in BOARDS]
        order_definitions = {
            CLOSE_CONTACTORS: [],         # Close contactors
            OPEN_CONTACTORS: [],          # Open contactors
            BRAKE: [],                    # Brake
            UNBRAKE: [],                  # Unbrake
            START_VERTICAL_LEVITATION: ["uint16_t"],  # Vertical Levitation variable = Desired_distance
            STOP_VERTICAL_LEVITATION: [],   # Stop Vertical Levitation
            START_HORIZONTAL_LEVITATION: [],  # Start Horizontal Levitation
            STOP_HORIZONTAL_LEVITATION: [],   # Stop Horizontal Levitation
            START_PROPULSION: ["uint16_t"],         # Start Propulsion variable = Desired_Distance
            STOP_PROPULSION: [],          # Stop Propulsion
            START_CHARGING_LV: [],        # Start Charging LV
            STOP_CHARGING_LV: [],         # Stop Charging LV
            START_CHARGING_G2V: [],       # Start Charging G2V
            STOP_CHARGING_G2V: [],        # Stop Charging G2V
            ENABLES_BOOSTER: [],
            DISABLES_BOOSTER: [],
            END_OF_RUN: [],
            STICK_DOWN: []
        }
        self.orders = Packets(order_definitions)
        self._enable = DigitalOutService(self._shm, Pinout.PF4)
        self._reeds = [
                DigitalOutService(Pinout.PD11,self._shm), #reed1
                DigitalOutService(Pinout.PD10,self._shm), #reed2
                DigitalOutService(Pinout.PG2,self._shm), #reed3
                DigitalOutService(Pinout.PG3,self._shm), #reed4
                DigitalOutService(Pinout.PD8,self._shm), #reed5
                DigitalOutService(Pinout.PD9,self._shm), #reed6
                DigitalOutService(Pinout.PB15,self._shm), #reed7
                DigitalOutService(Pinout.PB14,self._shm) #reed8    
        ]
        #Here we will have to add every single ADC and other inputs that receives the VCU
        print("VCU initialized succesfully")

    def connect_gui(self):
        self._gui.connect()

    def check_connection_boards(self) ->bool:
        for board in self._board_server_sockets:
            if board.get_connected_clients == None:
                return False
        return True
    def disconnect_gui(self):
        self._gui.stop()

    def disconnect_one_board(self,board : BOARD):
         for server_socket in self._boards_server_sockets:
            if (server_socket.local_ip, server_socket.local_port) == (board.address, board.port):
                server_socket.stop()
                break  
            
    def disconnect_boards(self):
        for server_socket in self._boards_server_sockets:
            server_socket.stop()
            
    def connect_one_board(self,board: BOARD):
        for server_socket in self._boards_server_sockets:
            if (server_socket.local_ip, server_socket.local_port) == (board.address, board.port):
                server_socket.stop()

    def connect_boards(self):
        for server_socket in self._boards_server_sockets:
            server_socket.start_server()

    def is_state_general(self, state: int) -> bool:
        current_state = self._shm.get_state_machine_state(1)
        return current_state == state
    
    def is_state_operational(self,state: int) -> bool:
        current_state = self._shm.get_state_machine_state(2)
        return current_state == state
    
    
    def is_braked(self) -> bool:
        for reed in self._reeds:
            if reed.get_pin_state() == DigitalOut.State.High:
                return False
        return True

    #To check where to send the order, I will check the list boards_server
    #if not check if the board is self._gui 
    def send_order(self,board : BOARD,id_order,*values):
        data = self.orders.serialize(id_order,*values)
        for server_socket in self._boards_server_sockets:
            if (server_socket.local_ip, server_socket.local_port) == (board.address, board.port):
                server_socket.transmit(data,(VCU_address.address,VCU_address.port))
                return
        if (self._gui.local_ip,self._gui.local_port) == (board.address, board.port):
                self._gui.transmit(self.orders.serialize(id_order,values))
                return
    
    #The idea of this function is to only check if a board has received
    #the order with the id
    def check_received_order(self, id_order_received) -> bool: 
        for server_socket in self._boards_server_sockets:
            if (server_socket.local_ip, server_socket.local_port) == (board.address, board.port):
                packet = server_socket.get_packet() 
                if (packet == None):
                    return False
                received_id, = struct.unpack('<H', packet[:2])
                return received_id == id_order_received
        return False

    def check_not_received_order(self) -> bool:
        for server_socket in self._boards_server_sockets:
            packet = server_socket.get_packet() 
            if (packet != None):
                return False
        return True
        
