import time
import os
from pymavlink import mavutil

# Dialecte généré
import rocket_dialect
os.environ["MAVLINK20"] = "1"
os.environ["MAV_IGNORE_CRC"] = "0"

# Serial connection
master = mavutil.mavlink_connection('COM14', baud=115200, source_system=1)

# Init MAVLink engine
master.mav = rocket_dialect.MAVLink(master, srcSystem=1, srcComponent=1)


counter = 0
print("Ground Station lancée. Écoute active...")

while True:
    # On attend un message de manière efficace
    msg = master.recv_match(blocking=True, timeout=0.5)
    if msg is None:
        continue
    print(f"Message reçu : {msg.get_type()}")
    
    msg_type = msg.get_type()

    if msg_type == 'BAD_DATA':
        if mavutil.all_printable(msg.data):
             print(f"⚠️ Erreur de trame (Char) : {msg.data}")
        continue

    source_id = msg.get_srcSystem()
    
    # Identification source
    if source_id == 2:
        prefix = "🟢 [AIR 1]"
    elif source_id == 3:
        prefix = "🔵 [AIR 2]"
    else:
        prefix = f"⚪ [SRC {source_id}]"

    # Logique de parsing
    if msg_type == 'STATUSTEXT':
        text_content = msg.text.replace('\x00', '').strip()
        print(f"{prefix} MSG : {text_content}")

    elif msg_type == 'ROCKET_TELEMETRY':
        print(f"\n--- {prefix} TRAME {time.strftime('%H:%M:%S')} ---")
        data = msg.to_dict()
        
        # Nettoyage et affichage
        for field, val in data.items():
            if field not in ['mavpackettype']:
                print(f"  > {field.ljust(15)} : {val}")
        print("-" * 30)