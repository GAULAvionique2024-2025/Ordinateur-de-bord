import os
os.environ["MAVLINK20"] = "1"

import time
from pymavlink import mavutil
import odb_mavlink_v1 as mavlink_dialect

# --- CONFIGURATION ---
SERIAL_PORT = 'COM15'
#SERIAL_PORT = 'udpout:127.0.0.1:14550'
BAUD_RATE = 115200
BOOSTER_SYSTEM_ID = 2
SUSTAINER_SYSTEM_ID = 3

def run_transmitter():
    master = mavutil.mavlink_connection(SERIAL_PORT, baud=BAUD_RATE)
    
    # Instances MAVLink séparées pour chaque airframe.
    booster_mav = mavlink_dialect.MAVLink(master, srcSystem=BOOSTER_SYSTEM_ID, srcComponent=1)
    sustainer_mav = mavlink_dialect.MAVLink(master, srcSystem=SUSTAINER_SYSTEM_ID, srcComponent=1)

    print(f"Transmission lancée sur {SERIAL_PORT}...")    
    start_time = time.time()

    while True:
        current_ms = int((time.time() - start_time) * 1000)
        
        # --- ENVOI BOOSTER (ID 1) ---
        booster_mav.rocket_telemetry_send(
            time_boot_ms=current_ms,
            system_states=0xFFFF,
            event_states=0x01,
            mission_state=1,
            battery_mv=12400,
            # Converted units for MAVLink fields (see XML): cdeg, cm/s/s, cdeg/s, cuT, cm, cdegC
            roll=5.00 * 100.0,          # 5 deg -> 500 cdeg
            pitch=10.00 * 100.0,        # 10 deg -> 1000 cdeg
            yaw=45.00 * 100.0,          # 45 deg -> 4500 cdeg
            imu_acc_x=0.00 * 100.0,     # 0 m/s2 -> 0 cm/s2
            imu_acc_y=0.00 * 100.0,
            imu_acc_z=9.81 * 100.0,     # 9.81 m/s2 -> 981 cm/s2
            imu_gyro_x=0.10 * 100.0,    # 0.10 deg/s -> 10 cdeg/s
            imu_gyro_y=0.20 * 100.0,
            imu_gyro_z=0.30 * 100.0,
            imu_mag_x=12.3 * 100.0,     # 12.3 uT -> 1230 cuT
            imu_mag_y=45.6 * 100.0,
            imu_mag_z=78.9 * 100.0,
            altitude_msl_m=1200.0 * 100.0, # 1200 m -> 120000 cm
            pressure_pa=980.5,
            temp_celsius=25.0 * 100.0,  # 25 C -> 2500 cdegC
            highg_acc_x=0.00 * 100.0,
            highg_acc_y=0.00 * 100.0,
            highg_acc_z=98.10 * 100.0,
            gps_fix=1,
            lat=465000000,
            lon=66000000,
            gps_alt=1200000,
            vel=150,
            cog=350,
            satellites_nb=12,
            imu_acc_vertical=9.81 * 100.0,
            highg_acc_vertical=98.10 * 100.0,
            kalman_z=1198.4 * 100.0,
            kalman_v=0.0 * 100.0,
        )
        print(f"[{current_ms}ms] Packet envoyé : BOOSTER (ID {BOOSTER_SYSTEM_ID})")

        time.sleep(0.10)

        # --- ENVOI SUSTAINER (ID 2) ---
        sustainer_mav.rocket_telemetry_send(
            time_boot_ms=current_ms,
            system_states=0x0F,
            event_states=0x02,
            mission_state=2,
            battery_mv=11800,
            # Converted units for MAVLink fields
            roll=0.00 * 100.0,
            pitch=89.00 * 100.0,
            yaw=0.00 * 100.0,
            imu_acc_x=0.00 * 100.0,
            imu_acc_y=0.00 * 100.0,
            imu_acc_z=4.50 * 100.0,
            imu_gyro_x=0.50 * 100.0,
            imu_gyro_y=0.50 * 100.0,
            imu_gyro_z=5.00 * 100.0,
            imu_mag_x=13.0 * 100.0,
            imu_mag_y=46.0 * 100.0,
            imu_mag_z=79.0 * 100.0,
            altitude_msl_m=8500.0 * 100.0,
            pressure_pa=750.2,
            temp_celsius=18.0 * 100.0,
            highg_acc_x=0.00 * 100.0,
            highg_acc_y=0.00 * 100.0,
            highg_acc_z=45.00 * 100.0,
            gps_fix=1,
            lat=465010000,
            lon=66005000,
            gps_alt=8500000,
            vel=45000,
            cog=0,
            satellites_nb=10,
            imu_acc_vertical=4.50 * 100.0,
            highg_acc_vertical=45.00 * 100.0,
            kalman_z=8498.7 * 100.0,
            kalman_v=-12.3 * 100.0,
        )
        print(f"[{current_ms}ms] Packet envoyé : SUSTAINER (ID {SUSTAINER_SYSTEM_ID})")
        time.sleep(0.40)

if __name__ == "__main__":
    run_transmitter()