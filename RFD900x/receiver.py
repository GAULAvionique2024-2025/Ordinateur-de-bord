import os
os.environ["MAVLINK20"] = "1"
os.environ.pop("MAV_IGNORE_CRC", 1)

import time
from pymavlink import mavutil
import odb_mavlink_v1 as mavlink_dialect

mavutil.mavlink = mavlink_dialect
mavutil.current_dialect = "odb_mavlink_v1"

# --- CONFIGURATION ---
SERIAL_PORT = "COM5"
#SERIAL_PORT = "udpin:0.0.0.0:14550"
BAUD_RATE = 115200
SOURCE_SYSTEM = 1
BOOSTER_SYS_ID = 2
SUSTAINER_SYS_ID = 3


def run_receiver():
    try:
        master = mavutil.mavlink_connection(
            SERIAL_PORT,
            baud=BAUD_RATE,
            source_system=SOURCE_SYSTEM,
        )
        master.mav.robust_parsing = True
        print(f"Station Sol active sur {SERIAL_PORT}")

        while True:
            try:
                msg = master.recv_match(blocking=True)
            except (OSError, IOError) as exc:
                print(f"\n[ERREUR LIAISON SERIE] {exc}")
                break

            if not msg:
                continue

            msg_type = msg.get_type()
            sys_id = msg.get_srcSystem()

            if msg_type == "BAD_DATA":
                raw_payload = msg.get_msgbuf()
                print(f"\n\033[91m[ERREUR CRC/FORMAT]\033[0m Reçu : {raw_payload.hex(' ')}")
                continue

            if msg_type == "ROCKET_TELEMETRY":
                data = msg.to_dict()

                nom = (
                    "BOOSTER"
                    if sys_id == BOOSTER_SYS_ID
                    else "SUSTAINER"
                    if sys_id == SUSTAINER_SYS_ID
                    else f"SYS_{sys_id}"
                )
                color = (
                    "\033[94m"
                    if sys_id == BOOSTER_SYS_ID
                    else "\033[92m"
                    if sys_id == SUSTAINER_SYS_ID
                    else "\033[93m"
                )

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
                pressure_hpa = data.get("pressure_hpa")
                altitude_msl_m = data.get("altitude_msl_m")
                temp_celsius = data.get("temp_celsius")
                imu_acc_x = data.get("imu_acc_x")
                imu_acc_y = data.get("imu_acc_y")
                imu_acc_z = data.get("imu_acc_z")
                imu_gyro_x = data.get("imu_gyro_x")
                imu_gyro_y = data.get("imu_gyro_y")
                imu_gyro_z = data.get("imu_gyro_z")
                imu_mag_x = data.get("imu_mag_x")
                imu_mag_y = data.get("imu_mag_y")
                imu_mag_z = data.get("imu_mag_z")
                highg_acc_x = data.get("highg_acc_x")
                highg_acc_y = data.get("highg_acc_y")
                highg_acc_z = data.get("highg_acc_z")
                imu_acc_vertical = data.get("imu_acc_vertical")
                highg_acc_vertical = data.get("highg_acc_vertical")
                kalman_z = data.get("kalman_z")
                kalman_v = data.get("kalman_v")
                system_states = data.get("system_states")
                event_states = data.get("event_states")

                print(f"\n{color}{'='*18} {nom} | MAVLink {msg_type} | {time.strftime('%H:%M:%S')} {'='*18}\033[0m")

                if lat is not None and lon is not None:
                    print(f"  Position      : {lat / 1e7:.7f}, {lon / 1e7:.7f}")

                if gps_alt is not None:
                    print(f"  GPS altitude   : {gps_alt / 1000.0:.2f} m")

                if altitude_msl_m is not None:
                    # altitude_msl_m sent as cm in MAVLink XML -> convert to meters
                    print(f"  Altitude MSL   : {altitude_msl_m / 100.0:.2f} m")

                if roll is not None and pitch is not None and yaw is not None:
                    # roll/pitch/yaw are sent as cdeg (centi-degrees)
                    print(f"  Attitude       : roll {roll/100.0:.2f} deg | pitch {pitch/100.0:.2f} deg | yaw {yaw/100.0:.2f} deg")

                if vel is not None:
                    # vel is in cm/s
                    print(f"  Vitesse sol    : {vel / 100.0:.2f} m/s")

                if battery_mv is not None:
                    print(f"  Batterie       : {battery_mv / 1000.0:.2f} V")

                if gps_fix is not None or satellites_nb is not None:
                    print(f"  GPS            : fix={gps_fix} | satellites={satellites_nb}")

                if mission_state is not None:
                    print(f"  Mission state  : {mission_state}")

                if system_states is not None:
                    print(f"  System states  : 0x{system_states:04X}")

                if event_states is not None:
                    print(f"  Event states   : 0x{event_states:04X}")

                if pressure_hpa is not None:
                    print(f"  Pression       : {pressure_hpa:.2f} hPa")

                if temp_celsius is not None:
                    # temp_celsius sent as cdegC (centi-degrees C)
                    print(f"  Température    : {temp_celsius / 100.0:.2f} °C")

                if imu_acc_x is not None and imu_acc_y is not None and imu_acc_z is not None:
                    # imu accelerations sent as cm/s/s -> convert to m/s^2
                    print(f"  IMU Acc        : x={imu_acc_x/100.0:.2f} y={imu_acc_y/100.0:.2f} z={imu_acc_z/100.0:.2f} m/s²")

                if imu_gyro_x is not None and imu_gyro_y is not None and imu_gyro_z is not None:
                    # gyro sent as cdeg/s -> convert to deg/s
                    print(f"  IMU Gyro       : x={imu_gyro_x/100.0:.2f} y={imu_gyro_y/100.0:.2f} z={imu_gyro_z/100.0:.2f} deg/s")

                if imu_mag_x is not None and imu_mag_y is not None and imu_mag_z is not None:
                    # magnetometer sent as cuT -> convert to uT
                    print(f"  IMU Mag        : x={imu_mag_x/100.0:.2f} y={imu_mag_y/100.0:.2f} z={imu_mag_z/100.0:.2f} uT")

                if highg_acc_x is not None and highg_acc_y is not None and highg_acc_z is not None:
                    # high-g accelerations in cm/s/s -> convert to m/s^2
                    print(f"  High-G Acc     : x={highg_acc_x/100.0:.2f} y={highg_acc_y/100.0:.2f} z={highg_acc_z/100.0:.2f} m/s²")

                if imu_acc_vertical is not None:
                    print(f"  IMU vert       : {imu_acc_vertical/100.0:.2f} m/s²")

                if highg_acc_vertical is not None:
                    print(f"  High-G vert    : {highg_acc_vertical/100.0:.2f} m/s²")

                if kalman_z is not None:
                    # kalman_z sent as cm -> convert to meters
                    print(f"  Kalman Z       : {kalman_z/100.0:.2f} m")

                if kalman_v is not None:
                    # kalman_v sent as cm/s -> convert to m/s
                    print(f"  Kalman V       : {kalman_v/100.0:.2f} m/s")

                print(f"{color}{'='*52}\033[0m")
                continue

            print(f"\n[RAW] Type non géré : {msg_type} (Source ID: {sys_id})")
            print(f"  Contenu : {msg.to_dict()}")

    except KeyboardInterrupt:
        print("\nArrêt de la station sol.")
    except Exception as exc:
        print(f"\n[ERREUR FATALE] {exc}")


if __name__ == "__main__":
    run_receiver()