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
            lat=465000000,
            lon=66000000,
            gps_alt=1200000,
            pressure_hpa=980.5,
            imu_gyro_x=10, 
            imu_gyro_y=20, 
            imu_gyro_z=30,
            highg_acc_x=0, 
            highg_acc_y=0, 
            highg_acc_z=9810,    # 1000 * 9.81 m/s^2 ? (selon ton unité en XML)
            roll=500, 
            pitch=1000, 
            yaw=4500,
            temp_celsius=2500,   # 25.00 °C
            imu_acc_x=0, 
            imu_acc_y=0, 
            imu_acc_z=981,
            imu_mag_x=123, 
            imu_mag_y=456, 
            imu_mag_z=789,
            system_states=0xFFFF, # Tous les flags OK
            battery_mv=12400,     # 12.4V
            vel=150, 
            cog=350,
            event_states=0x01,    # Premier flag d'événement
            mission_state=1,      # READY
            gps_fix=1,
            satellites_nb=12
        )
        print(f"[{current_ms}ms] Packet envoyé : BOOSTER (ID {BOOSTER_SYSTEM_ID})")

        # --- ENVOI SUSTAINER (ID 2) ---
        sustainer_mav.rocket_telemetry_send(
            time_boot_ms=current_ms,
            lat=465010000,          # Latitude un peu décalée
            lon=66005000,           # Longitude un peu décalée
            gps_alt=8500000,        # 8500 mètres (en mm)
            pressure_hpa=750.2,     # Pression plus basse
            imu_gyro_x=50,          # Rotation plus forte
            imu_gyro_y=50, 
            imu_gyro_z=500,
            highg_acc_x=0, 
            highg_acc_y=0, 
            highg_acc_z=4500,       # 4.5G de poussée
            roll=0, 
            pitch=8900,             # Presque à la verticale (89 deg)
            yaw=0,
            temp_celsius=1800,      # Plus froid en altitude (18.00 C)
            imu_acc_x=0, 
            imu_acc_y=0, 
            imu_acc_z=4500,
            imu_mag_x=130, 
            imu_mag_y=460, 
            imu_mag_z=790,
            system_states=0x0F, 
            battery_mv=11800,       # Batterie un peu plus faible
            vel=45000,              # 450 m/s (en cm/s)
            cog=0,
            event_states=0x02,      # Autre flag d'événement
            mission_state=2,        # IN_FLIGHT
            gps_fix=1, 
            satellites_nb=10
        )
        print(f"[{current_ms}ms] Packet envoyé : SUSTAINER (ID {SUSTAINER_SYSTEM_ID})")
        time.sleep(0.25)

if __name__ == "__main__":
    run_transmitter()