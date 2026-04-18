import time
from pymavlink import mavutil

master = mavutil.mavlink_connection(
    'COM15', 
    baud=115200, 
    source_system=2
)

counter = 0
while True:
    master.mav.heartbeat_send(mavutil.mavlink.MAV_TYPE_ROCKET, mavutil.mavlink.MAV_AUTOPILOT_INVALID, 0, 0, 0)

    msg_payload = f"Air1-30k: Status OK - {counter}"
    master.mav.statustext_send(mavutil.mavlink.MAV_SEVERITY_INFO, msg_payload.encode('utf-8'))
    
    print(f"Envoi Air 1: {msg_payload}")
    counter += 1
    time.sleep(0.5)