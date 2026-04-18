import time
from pymavlink import mavutil

# 1. Connexion
master = mavutil.mavlink_connection(
    'COM14', 
    baud=115200, 
    source_system=1
)
print("Ground Station lancée. Écoute des modules Air 1 et Air 2...")

while True:
    msg = master.recv_match(blocking=False)
    if msg:
        msg_type = msg.get_type()
        if msg_type == 'STATUSTEXT':
            source_id = msg.get_srcSystem()
            texte_recu = msg.text.strip('\x00')
            
            if source_id == 2:
                print(f"🟢 [AIR 1 - 30k] : {texte_recu}")
            elif source_id == 3:
                print(f"🔵 [AIR 2 - 90k] : {texte_recu}")
            else:
                print(f"⚪ [SOURCE {source_id}] : {texte_recu}")

    time.sleep(0.01)