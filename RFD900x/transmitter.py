import time
from pymavlink import mavutil

# Connection Air 1 (ID = 1)
master = mavutil.mavlink_connection(
    'com1', 
    baud = 115200, 
    source_system = 1
)
master.wait_heartbeat()
print("AIR 1 Connected. Beginning transmission...")

counter = 0

while True:
    # Heartbeat
    master.mav.heartbeat_send(
        mavutil.mavlink.MAV_TYPE_ROCKET,
        mavutil.mavlink.MAV_AUTOPILOT_INVALID,
        0, 0, 0
    )

    # Message
    message = f"helloworld1 - msg {counter}".encode('utf-8')
    master.mav.statustext_send(
        mavutil.mavlink.MAV_SEVERITY_INFO,
        message
    )
    
    #print(f"Sended : {message.decode('utf-8')}")
    counter += 1
    
    time.sleep(0.5)