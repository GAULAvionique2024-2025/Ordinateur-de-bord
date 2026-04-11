import time
from pymavlink import mavutil

# Connection Ground Station (ID = 255)
master = mavutil.mavlink_connection(
    'com4',
    baud=57600, 
    source_system=255
)
print("Ground Station Connected. Waiting for messages...")

while True:
    # Message
    msg = master.recv_match(blocking=False)
    if msg:
        msg_type = msg.get_type()
        if msg_type == 'STATUSTEXT':
            source_id = msg.get_srcSystem()
            texte_recu = msg.text.strip('\x00')
            if source_id == 1:
                print(f"🟢 [RECEIVED FROM AIR 1 - 30k ft] : {texte_recu}")
            elif source_id == 2:
                print(f"🔵 [RECEIVED FROM AIR 2 - 100k ft] : {texte_recu}")
            else:
                print(f"⚪ [RECEIVED FROM UNKNOWN SOURCE {source_id}] : {texte_recu}")

    time.sleep(0.01)