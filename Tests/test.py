import sys, os
sys.path.append(os.path.join(os.path.dirname(__file__), "VirtualMCU", "src"))
from time import sleep
from runner import runner, LOG
from vmcu.shared_memory import SharedMemory
from vmcu.pin import Pinout, DigitalOut
from vmcu.services.digital_out import DigitalOutService
from vmcu.assertions import *
from VCU import *
from utility import *
@runner.test()
def Test_VCU_Entire():
    print("We will test the VCU logic during this test")
   
    
    vcu = VCU()
    print("before starting test checking that the number state machines is 2")
    check(check_number_of_active_state_machines, args=(vcu._shm, 2), msg="Expected 2 state machines")

    print("Test 1: Connection to the boards\n We will connect one by one the boards and check if changes to operational when it has to change")
    for board in BOARDS:
        vcu.connect_one_board(board)
        check(vcu.is_state_general,args=(General_SM.CONNECTING),msg="Expected connected read something else")

   
    
    vcu.connect_gui()
    sleep(1) #One second delay so the vcu has time to connect to all the boards
    
    completes(
        wait_until_true(vcu.is_state_general),
        args=(General_SM.OPERATIONAL),
        before=seconds(4),
        msg="Should be in operational but is not in operational"
    )
    print("Checked that once everything connected goes to operational")
    check(vcu.is_state_operational,args=(Operational_SM.IDLE),msg = "VCU is not in the correct state")
    print("Checked that is in IDLE once change to operational")
    
    print("Test 2: Let's check the state Machine how well it works")
    print("Send orders that cannot be send in IDLE and check if anything happens")
    for order in ORDERS_VCU:
        if order not in ORDERS_IDLE and order not in ORDERS_WITH_ARGS:
            vcu.send_order(GUI,order)
            sleep(0.2)
            #check if the vcu has changed the state and if any board has received a message
            check(vcu.check_not_received_order,msg = f"Received order with id {order} to a board, that shouldn't be received in IDLE")
            
    
    
    
runner.run() # Runs the tests, do not delete!
