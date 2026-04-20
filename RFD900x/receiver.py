import os
os.environ["MAVLINK20"] = "1"
os.environ.pop("MAV_IGNORE_CRC", None)

import time

from pymavlink import mavutil

import odb_mavlink_v1 as mavlink_dialect


# --- CONFIGURATION ---
#SERIAL_PORT = 'COM15'
SERIAL_PORT = 'udpin:0.0.0.0:14550'
BAUD_RATE = 115200
SOURCE_SYSTEM = 1
BOOSTER_SYS_ID = 1
SUSTAINER_SYS_ID = 2

def run_receiver():
    master = mavutil.mavlink_connection(SERIAL_PORT, baud=BAUD_RATE, source_system=SOURCE_SYSTEM)
    master.mav = mavlink_dialect.MAVLink(master, srcSystem=SOURCE_SYSTEM, srcComponent=1)

    print(f"Station Sol active sur {SERIAL_PORT}")

    while True:
        msg = master.recv_match(blocking=True, timeout=0.5)
        if not msg:
            continue

        msg_type = msg.get_type()
        sys_id = msg.get_srcSystem()

        if msg_type == 'BAD_DATA':
            raw_payload = msg.get_msgbuf()
            print(f"\n\033[91m[ERREUR CRC/FORMAT]\033[0m Reçu : {raw_payload.hex(' ')}")
            continue

        if msg_type == 'ROCKET_TELEMETRY':
            data = msg.to_dict()
            # STM32 side uses modem IDs: BOOSTER=1, SUSTAINER=2.
            nom = "BOOSTER" if sys_id == BOOSTER_SYS_ID else "SUSTAINER" if sys_id == SUSTAINER_SYS_ID else f"SYS_{sys_id}"
            color = "\033[94m" if sys_id == BOOSTER_SYS_ID else "\033[92m" if sys_id == SUSTAINER_SYS_ID else "\033[93m"

            lat = data.get("lat")
            lon = data.get("lon")
            gps_alt = data.get("gps_alt")
            roll = data.get("roll")
            pitch = data.get("pitch")
            yaw = data.get("yaw")
            battery_mv = data.get("battery_mv")
            vel = data.get("vel")
            mission_state = data.get("mission_state")
            gps_fix = data.get("gps_fix")
            satellites_nb = data.get("satellites_nb")

            print(f"\n{color}{'='*18} {nom} | MAVLink {msg_type} | {time.strftime('%H:%M:%S')} {'='*18}\033[0m")
            if lat is not None and lon is not None:
                print(f"  Position      : {lat / 1e7:.7f}, {lon / 1e7:.7f}")
            if gps_alt is not None:
                print(f"  Altitude      : {gps_alt / 1000.0:.2f} m")
            if roll is not None and pitch is not None and yaw is not None:
                print(f"  Attitude      : roll {roll / 100:.2f} deg | pitch {pitch / 100:.2f} deg | yaw {yaw / 100:.2f} deg")
            if vel is not None:
                print(f"  Vitesse sol   : {vel / 100.0:.2f} m/s")
            if battery_mv is not None:
                print(f"  Batterie      : {battery_mv / 1000.0:.2f} V")
            if gps_fix is not None or satellites_nb is not None:
                print(f"  GPS           : fix={gps_fix} | satellites={satellites_nb}")
            if mission_state is not None:
                print(f"  Mission state : {mission_state}")

            pressure_hpa = data.get("pressure_hpa")
            if pressure_hpa is not None:
                print(f"  Pression      : {pressure_hpa:.2f} hPa")

            print(f"{color}{'='*52}\033[0m")
            continue

        else:
            print(f"\n[RAW] Type non géré : {msg_type} (Source ID: {sys_id})")
            print(f"  Contenu : {msg.to_dict()}")

if __name__ == "__main__":
    try:
        run_receiver()
    except KeyboardInterrupt:
        print("\nArrêt de la station sol.")