import csv
import ctypes


PYRO_ROLE_NAMES = {
    0: 'Aucun',
    1: 'Main',
    2: 'Drogue',
    3: 'Main backup',
    4: 'Drogue backup',
}

MISSION_STATE_NAMES = {
    0: 'Prévol',
    1: 'Vol',
    2: 'Postvol',
}

SYSTEM_FLAG_NAMES = [
    (14, 'IdeFIX communication OK'),
    (13, 'Bluetooth module OK'),
    (12, 'Flash memory OK'),
    (11, 'SD card OK'),
    (10, 'Temperature sensor OK'),
    (9, 'High-G accelerometer OK'),
    (8, 'GPS OK'),
    (7, 'Barometer OK'),
    (6, 'IMU OK'),
    (5, 'Radio OK'),
    (4, 'Pyros armed OK'),
    (3, 'Pyro arming module OK'),
    (2, 'Pyro 2 connected'),
    (1, 'Pyro 3 connected'),
    (0, 'Pyro 4 connected'),
]

EVENT_FLAG_NAMES = [
    (8, 'Mach lock enabled'),
    (7, 'Drogue deployed'),
    (6, 'Main deployed'),
    (5, 'Apogee detected'),
    (4, 'Pyro 4 fired'),
    (3, 'Pyro 3 fired'),
    (2, 'Pyro 2 fired'),
    (1, 'Pyro 1 fired'),
    (0, 'Pyros armed'),
]


class OdbConfigPacket(ctypes.LittleEndianStructure):
    _pack_ = 1
    _fields_ = [
        ('magic_number', ctypes.c_uint32),
        ('version_major', ctypes.c_uint8),
        ('version_minor', ctypes.c_uint8),
        ('payload_size', ctypes.c_uint16),
        ('odb_name', ctypes.c_char * 32),
        ('stage_role', ctypes.c_uint8),
        ('debug_mode', ctypes.c_uint8),
        ('fire_attempt_delay_ms', ctypes.c_uint32),
        ('pyros_arming_failsafe_ticks', ctypes.c_uint32),
        ('min_needed_pyro_nb', ctypes.c_uint8),
        ('pyro_roles', ctypes.c_uint8 * 4),
        ('acc_z_launch_threshold', ctypes.c_float),
        ('boost_phase_v_threshold', ctypes.c_float),
        ('apogee_detect_v_threshold', ctypes.c_float),
        ('landing_detect_v_threshold', ctypes.c_float),
        ('landing_detect_threshold_ms', ctypes.c_uint32),
        ('apogee_failsafe_ticks', ctypes.c_uint32),
        ('main_deploy_altitude_threshold_m', ctypes.c_float),
        ('drogue_fire_attempt_max_nb', ctypes.c_uint8),
        ('main_fire_attempt_max_nb', ctypes.c_uint8),
        ('enable_buzzer', ctypes.c_uint8),
        ('buzzer_report_tone_hz', ctypes.c_uint16),
        ('idefix_frequency_hz', ctypes.c_uint32),
    ]


class PyroEventPacket(ctypes.LittleEndianStructure):
    _pack_ = 1
    _fields_ = [
        ('fired', ctypes.c_bool),
        ('time_ms', ctypes.c_uint32),
    ]


class WindowEventPacket(ctypes.LittleEndianStructure):
    _pack_ = 1
    _fields_ = [
        ('activated', ctypes.c_bool),
        ('start_time_ms', ctypes.c_uint32),
        ('end_time_ms', ctypes.c_uint32),
    ]


class MetricPacket(ctypes.LittleEndianStructure):
    _pack_ = 1
    _fields_ = [
        ('valid', ctypes.c_bool),
        ('value', ctypes.c_float),
        ('time_ms', ctypes.c_uint32),
    ]


class OdbStatsPacket(ctypes.LittleEndianStructure):
    _pack_ = 1
    _fields_ = [
        ('pyro1', PyroEventPacket),
        ('pyro2', PyroEventPacket),
        ('pyro3', PyroEventPacket),
        ('pyro4', PyroEventPacket),
        ('pyros_arm', WindowEventPacket),
        ('mach_lock', WindowEventPacket),
        ('max_altitude_gps', MetricPacket),
        ('max_altitude_baro', MetricPacket),
        ('max_altitude_kalman', MetricPacket),
        ('apogee', MetricPacket),
        ('main_deploy', MetricPacket),
        ('drogue_deploy', MetricPacket),
        ('max_ascend_speed', MetricPacket),
        ('max_ascend_accel', MetricPacket),
        ('max_descend_speed', MetricPacket),
        ('max_descend_accel', MetricPacket),
        ('last_lat', ctypes.c_int32),
        ('last_lon', ctypes.c_int32),
        ('flight_time_ms', ctypes.c_uint32),
    ]


class OdbDataPacket(ctypes.LittleEndianStructure):
    _pack_ = 1
    _fields_ = [
        ('version_major', ctypes.c_uint8),
        ('version_minor', ctypes.c_uint8),
        ('payload_size', ctypes.c_uint16),
        ('time_boot_ms', ctypes.c_uint32),
        ('system_states', ctypes.c_uint16),
        ('event_states', ctypes.c_uint16),
        ('mission_state', ctypes.c_uint8),
        ('battery_mv', ctypes.c_uint16),
        ('roll', ctypes.c_float),
        ('pitch', ctypes.c_float),
        ('yaw', ctypes.c_float),
        ('imu_acc_x', ctypes.c_float),
        ('imu_acc_y', ctypes.c_float),
        ('imu_acc_z', ctypes.c_float),
        ('imu_gyro_x', ctypes.c_float),
        ('imu_gyro_y', ctypes.c_float),
        ('imu_gyro_z', ctypes.c_float),
        ('imu_mag_x', ctypes.c_float),
        ('imu_mag_y', ctypes.c_float),
        ('imu_mag_z', ctypes.c_float),
        ('altitude_msl_m', ctypes.c_float),
        ('pressure_pa', ctypes.c_float),
        ('temp_celsius', ctypes.c_float),
        ('highg_acc_x', ctypes.c_float),
        ('highg_acc_y', ctypes.c_float),
        ('highg_acc_z', ctypes.c_float),
        ('gps_fix', ctypes.c_uint8),
        ('lat', ctypes.c_int32),
        ('lon', ctypes.c_int32),
        ('gps_alt', ctypes.c_int32),
        ('vel', ctypes.c_uint16),
        ('cog', ctypes.c_uint16),
        ('satellites_nb', ctypes.c_uint8),
        ('imu_acc_vertical', ctypes.c_float),
        ('highg_acc_vertical', ctypes.c_float),
        ('kalman_z', ctypes.c_float),
        ('kalman_v', ctypes.c_float),
        ('padding', ctypes.c_uint8 * 11),
    ]


CONFIG_MAGIC = 0x434F4E46
HEADER_MAGIC = 0x48454144
STATS_MAGIC = 0x53544154
DATA_MAGIC = 0x44415441
NEW_DATA_PAYLOAD_SIZE = ctypes.sizeof(OdbDataPacket)
DATA_RECORD_SIZE = 4 + NEW_DATA_PAYLOAD_SIZE


def _bool_text(value):
    return 'Oui' if value else 'Non'


def _format_role(role):
    return f"{role} ({PYRO_ROLE_NAMES.get(role, 'Inconnu')})"


def _format_flags(mask, flag_names):
    enabled = [name for bit, name in flag_names if mask & (1 << bit)]
    if not enabled:
        return 'Aucun'
    return ', '.join(enabled)


def _format_metric(metric, precision=3):
    if not metric.valid:
        return 'Invalide'
    return f"{metric.value:.{precision}f} (à {metric.time_ms} ms)"


def _format_lat_lon(value):
    return f"{value / 1e7:.7f}"


def _format_sat_count(value):
    return str(int(value))


class W25QParser:
    def __init__(self, data):
        self.data = data
        self.sector_size = 4096
        self.page_size = 256
        self.data_record_size = DATA_RECORD_SIZE
        self.flights = []
        self.sector_map = {}
        self.config_details = []

    def scan_memory(self):
        """Analyse la mémoire pour mapper les secteurs et trouver les vols/config."""
        self.flights = []
        self.sector_map = {}
        data_len = len(self.data)
        sectors = data_len // self.sector_size

        for s in range(sectors):
            addr = s * self.sector_size
            if self.data[addr:addr + 4] == b'\xff\xff\xff\xff':
                self.sector_map[s] = 'empty'
            else:
                self.sector_map[s] = 'used'

        conf_magic_le = CONFIG_MAGIC.to_bytes(4, 'little')
        idx = self.data.find(conf_magic_le)
        if idx != -1:
            conf_sector = idx // self.sector_size
            self.sector_map[conf_sector] = 'config'
            self._parse_config(idx)

        for s in range(sectors):
            addr = s * self.sector_size
            if addr + 4 > data_len:
                break

            magic = int.from_bytes(self.data[addr:addr + 4], 'little')
            if magic == HEADER_MAGIC:
                flight_id = int.from_bytes(self.data[addr + 4:addr + 8], 'little')
                self.flights.append({'id': flight_id, 'header_addr': addr})
                self.sector_map[s] = 'header'

                stats_addr = addr + self.sector_size
                if stats_addr + 4 <= data_len and int.from_bytes(self.data[stats_addr:stats_addr + 4], 'little') == STATS_MAGIC:
                    self.sector_map[s + 1] = 'stats'

                data_start = addr + self.sector_size + self.page_size
                off = data_start
                while off + 4 <= data_len:
                    if int.from_bytes(self.data[off:off + 4], 'little') != DATA_MAGIC:
                        break
                    self.sector_map[off // self.sector_size] = 'data'
                    off += self.data_record_size

    def _parse_config(self, off):
        try:
            config = OdbConfigPacket.from_buffer_copy(self.data[off:off + ctypes.sizeof(OdbConfigPacket)])
            name = bytes(config.odb_name).split(b'\x00', 1)[0].decode('ascii', errors='replace')

            self.config_details = [
                ('Magic', f"0x{config.magic_number:08X}"),
                ('Version', f"{config.version_major}.{config.version_minor}"),
                ('Payload size', str(config.payload_size)),
                ("Nom ODB", name),
                ("Rôle Stage", _format_role(config.stage_role)),
                ("Mode Debug", _bool_text(config.debug_mode)),
                ("Délai Tentative Tir (ms)", str(config.fire_attempt_delay_ms)),
                ("Failsafe Armement Pyro (ticks)", str(config.pyros_arming_failsafe_ticks)),
                ("Pyros Min Requis", str(config.min_needed_pyro_nb)),
                ("Rôles Pyro", ", ".join(PYRO_ROLE_NAMES.get(role, str(role)) for role in config.pyro_roles)),
                ("Seuil Lancement (Acc Z)", f"{config.acc_z_launch_threshold:.3f}"),
                ("Seuil Vitesse Boost", f"{config.boost_phase_v_threshold:.3f}"),
                ("Seuil Détection Apogée", f"{config.apogee_detect_v_threshold:.3f}"),
                ("Seuil Détection Atterrissage (V)", f"{config.landing_detect_v_threshold:.3f}"),
                ("Seuil Détection Atterrissage (ms)", str(config.landing_detect_threshold_ms)),
                ("Failsafe Apogée (ticks)", str(config.apogee_failsafe_ticks)),
                ("Seuil Déploiement Principal (m)", f"{config.main_deploy_altitude_threshold_m:.3f}"),
                ("Tirs Drogue Max", str(config.drogue_fire_attempt_max_nb)),
                ("Tirs Principal Max", str(config.main_fire_attempt_max_nb)),
                ("Buzzer Activé", _bool_text(config.enable_buzzer)),
                ("Tonalité Buzzer (Hz)", str(config.buzzer_report_tone_hz)),
                ("Fréquence IdeFIX (Hz)", str(config.idefix_frequency_hz)),
            ]
        except Exception as e:
            self.config_details = [("Erreur", str(e))]

    def _read_stats_packet(self, stats_addr):
        return OdbStatsPacket.from_buffer_copy(self.data[stats_addr + 4:stats_addr + 4 + ctypes.sizeof(OdbStatsPacket)])

    def get_flight_stats(self, flight):
        stats_addr = flight['header_addr'] + self.sector_size
        entries = []
        if stats_addr + 4 <= len(self.data):
            if int.from_bytes(self.data[stats_addr:stats_addr + 4], 'little') == STATS_MAGIC:
                stats = self._read_stats_packet(stats_addr)

                for idx, pyro in enumerate((stats.pyro1, stats.pyro2, stats.pyro3, stats.pyro4), start=1):
                    entries.append((f'Pyro {idx} Déclenché', _bool_text(pyro.fired)))
                    entries.append((f'Pyro {idx} Temps (ms)', str(pyro.time_ms)))

                for title, window in (('Armement Pyros', stats.pyros_arm), ('Mach Lock', stats.mach_lock)):
                    entries.append((f'{title} Actif', _bool_text(window.activated)))
                    entries.append((f'{title} Début (ms)', str(window.start_time_ms)))
                    entries.append((f'{title} Fin (ms)', str(window.end_time_ms)))

                metric_entries = [
                    ('Max Alt GPS', stats.max_altitude_gps),
                    ('Max Alt Baro', stats.max_altitude_baro),
                    ('Max Alt Kalman', stats.max_altitude_kalman),
                    ('Apogée', stats.apogee),
                    ('Déploiement Principal', stats.main_deploy),
                    ('Déploiement Drogue', stats.drogue_deploy),
                    ('Vitesse Asc Max', stats.max_ascend_speed),
                    ('Accel Asc Max', stats.max_ascend_accel),
                    ('Vitesse Desc Max', stats.max_descend_speed),
                    ('Accel Desc Max', stats.max_descend_accel),
                ]
                for name, metric in metric_entries:
                    entries.append((name, _format_metric(metric)))

                entries.append(('Dernière Latitude', _format_lat_lon(stats.last_lat)))
                entries.append(('Dernière Longitude', _format_lat_lon(stats.last_lon)))
                entries.append(('Temps de vol total (ms)', str(stats.flight_time_ms)))
        return entries

    def get_flight_packets(self, flight):
        data_start = flight['header_addr'] + self.sector_size + self.page_size
        packets = []
        off = data_start

        while off + 4 <= len(self.data):
            if int.from_bytes(self.data[off:off + 4], 'little') != DATA_MAGIC:
                break
            odb_off = off + 4
            if odb_off + ctypes.sizeof(OdbDataPacket) > len(self.data):
                break
            pkt = OdbDataPacket.from_buffer_copy(self.data[odb_off:odb_off + ctypes.sizeof(OdbDataPacket)])

            packets.append({
                'Version Major': pkt.version_major,
                'Version Minor': pkt.version_minor,
                'Payload Size': pkt.payload_size,
                'Time (ms)': pkt.time_boot_ms,
                'System States': f"0x{pkt.system_states:04X}",
                'System Flags': _format_flags(pkt.system_states, SYSTEM_FLAG_NAMES),
                'Event States': f"0x{pkt.event_states:04X}",
                'Event Flags': _format_flags(pkt.event_states, EVENT_FLAG_NAMES),
                'Mission State': MISSION_STATE_NAMES.get(pkt.mission_state, str(pkt.mission_state)),
                'Battery (mV)': pkt.battery_mv,
                'Roll (deg)': f"{pkt.roll:.2f}",
                'Pitch (deg)': f"{pkt.pitch:.2f}",
                'Yaw (deg)': f"{pkt.yaw:.2f}",
                'IMU Acc X (m/s2)': f"{pkt.imu_acc_x:.3f}",
                'IMU Acc Y (m/s2)': f"{pkt.imu_acc_y:.3f}",
                'IMU Acc Z (m/s2)': f"{pkt.imu_acc_z:.3f}",
                'IMU Gyro X (deg/s)': f"{pkt.imu_gyro_x:.3f}",
                'IMU Gyro Y (deg/s)': f"{pkt.imu_gyro_y:.3f}",
                'IMU Gyro Z (deg/s)': f"{pkt.imu_gyro_z:.3f}",
                'IMU Mag X (uT)': f"{pkt.imu_mag_x:.3f}",
                'IMU Mag Y (uT)': f"{pkt.imu_mag_y:.3f}",
                'IMU Mag Z (uT)': f"{pkt.imu_mag_z:.3f}",
                'Alt MSL (m)': f"{pkt.altitude_msl_m:.3f}",
                'Pressure (Pa)': f"{pkt.pressure_pa:.2f}",
                'Temp (°C)': f"{pkt.temp_celsius:.2f}",
                'HighG Acc X (m/s2)': f"{pkt.highg_acc_x:.3f}",
                'HighG Acc Y (m/s2)': f"{pkt.highg_acc_y:.3f}",
                'HighG Acc Z (m/s2)': f"{pkt.highg_acc_z:.3f}",
                'GPS Fix': pkt.gps_fix,
                'Latitude (degE7)': pkt.lat,
                'Latitude (deg)': _format_lat_lon(pkt.lat),
                'Longitude (degE7)': pkt.lon,
                'Longitude (deg)': _format_lat_lon(pkt.lon),
                'GPS Alt (mm)': pkt.gps_alt,
                'Vel (cm/s)': pkt.vel,
                'COG (cdeg)': pkt.cog,
                'Satellites': _format_sat_count(pkt.satellites_nb),
                'IMU Acc Vertical (m/s2)': f"{pkt.imu_acc_vertical:.3f}",
                'HighG Acc Vertical (m/s2)': f"{pkt.highg_acc_vertical:.3f}",
                'Kalman Z (m)': f"{pkt.kalman_z:.3f}",
                'Kalman V (m/s)': f"{pkt.kalman_v:.3f}",
            })
            off += self.data_record_size
        return packets

    def export_flight_packets_csv(self, flight, file_path, delimiter=';'):
        packets = self.get_flight_packets(flight)
        if not packets:
            return 0

        headers = list(packets[0].keys())
        with open(file_path, 'w', newline='', encoding='utf-8-sig') as csv_file:
            writer = csv.DictWriter(csv_file, fieldnames=headers, delimiter=delimiter)
            writer.writeheader()
            writer.writerows(packets)

        return len(packets)

    def get_memory_summary(self):
        """Retourne un dictionnaire avec la taille en octets de chaque type de secteur."""
        summary = {'empty': 0, 'config': 0, 'header': 0, 'stats': 0, 'data': 0, 'used': 0}
        total_sectors = len(self.data) // self.sector_size

        for s in range(total_sectors):
            state = self.sector_map.get(s, 'empty')
            if state in summary:
                summary[state] += self.sector_size
            else:
                summary['used'] += self.sector_size

        return summary
