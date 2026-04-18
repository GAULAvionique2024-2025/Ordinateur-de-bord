import time
import os
from pymavlink import mavutil

# Dialecte généré
import rocket_dialect 

# Connexion série AVEC dialecte
master = mavutil.mavlink_connection('COM15', baud=115200, source_system=2,)

# Init MAVLink engine
master.mav = rocket_dialect.MAVLink(master, srcSystem=2, srcComponent=1)

print("Transmetteur [AIR 1] lancé...")
start_time = time.time()

while True:
    # Heartbeat
    master.mav.heartbeat_send(
        mavutil.mavlink.MAV_TYPE_ROCKET,
        mavutil.mavlink.MAV_AUTOPILOT_INVALID,
        0, 0, 0
    )

    # Custom telemetry
    master.mav.rocket_telemetry_send(
        time_boot_ms = int((time.time() - start_time) * 1000) & 0xFFFFFFFF,
        lat=467800000,
        lon=-712200000,
        gps_alt=15000,
        pressure_hpa=1013.25,
        imu_gyro_x=0,
        imu_gyro_y=0,
        imu_gyro_z=0,
        highg_acc_x=0,
        highg_acc_y=0,
        highg_acc_z=980,
        roll=0,
        pitch=0,
        yaw=0,
        temp_celsius=2500,
        imu_acc_x=0,
        imu_acc_y=0,
        imu_acc_z=980,
        imu_mag_x=100,
        imu_mag_y=100,
        imu_mag_z=100,
        system_states=1,
        battery_mv=4200,
        vel=0,
        cog=0,
        event_states=0,
        mission_state=1,
        gps_fix=3,
        satellites_nb=12
    )

    print("Trame envoyée")
    time.sleep(1.0)
