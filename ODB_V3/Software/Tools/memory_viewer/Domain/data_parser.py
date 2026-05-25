import struct
import csv
import numpy as np


class W25QParser:
    def __init__(self, data):
        self.data = data
        self.sector_size = 4096
        self.page_size = 256
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

        conf_magic_le = (0x434F4E46).to_bytes(4, 'little')
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
            if magic == 0x48454144:
                flight_id = int.from_bytes(self.data[addr + 4:addr + 8], 'little')
                self.flights.append({'id': flight_id, 'header_addr': addr})
                self.sector_map[s] = 'header'

                stats_addr = addr + self.sector_size
                if stats_addr + 4 <= data_len and int.from_bytes(self.data[stats_addr:stats_addr + 4], 'little') == 0x53544154:
                    self.sector_map[s + 1] = 'stats'

                data_start = addr + self.sector_size + self.page_size
                off = data_start
                while off + 4 <= data_len:
                    if int.from_bytes(self.data[off:off + 4], 'little') != 0x44415441:
                        break
                    self.sector_map[off // self.sector_size] = 'data'
                    off += 132

    def _parse_config(self, off):
        try:
            magic_number = int.from_bytes(self.data[off:off + 4], 'little')
            name = self.data[off + 4:off + 4 + 32].split(b'\x00', 1)[0].decode('ascii', errors='replace')
            stage_role = self.data[off + 36]
            debug_mode = self.data[off + 37]
            enable_buzzer = self.data[off + 38]
            floats = struct.unpack_from('<5f', self.data, off + 40)
            u32_4 = struct.unpack_from('<4I', self.data, off + 60)
            pyro_values = struct.unpack_from('<3B', self.data, off + 76)
            buzzer_report_tone_hz = struct.unpack_from('<H', self.data, off + 80)[0]

            self.config_details = [
                ("Magic number", f"0x{magic_number:08X}"),
                ("Nom ODB", name),
                ("Rôle", f"{stage_role} (2=BOOSTER, 3=SUSTAINER)"),
                ("Mode Debug", str(debug_mode)),
                ("Buzzer Activé", str(enable_buzzer)),
                ("Seuil Lancement (Acc Z)", f"{floats[0]:.3f}"),
                ("Seuil Vitesse Boost", f"{floats[1]:.3f}"),
                ("Seuil Détection Apogée", f"{floats[2]:.3f}"),
                ("Seuil Déploiement Principal (m)", f"{floats[3]:.3f}"),
                ("Seuil Vitesse Atterrissage", f"{floats[4]:.3f}"),
                ("Seuil Détection Atterrissage (ms)", str(u32_4[0])),
                ("Délai Tentative Tir (ms)", str(u32_4[1])),
                ("Failsafe Armement Pyro (ticks)", str(u32_4[2])),
                ("Failsafe Apogée (ticks)", str(u32_4[3])),
                ("Pyros Min Requis", str(pyro_values[0])),
                ("Tirs Drogue Max", str(pyro_values[1])),
                ("Tirs Principal Max", str(pyro_values[2])),
                ("Tonalité Buzzer (Hz)", str(buzzer_report_tone_hz)),
            ]
        except Exception as e:
            self.config_details = [("Erreur", str(e))]

    def get_flight_stats(self, flight):
        stats_addr = flight['header_addr'] + self.sector_size
        entries = []
        if stats_addr + 4 <= len(self.data):
            if int.from_bytes(self.data[stats_addr:stats_addr + 4], 'little') == 0x53544154:
                off = stats_addr + 4
                for idx in range(5):
                    fired = bool(self.data[off])
                    time_ms = int.from_bytes(self.data[off + 4:off + 8], 'little')
                    entries.append((f'Pyro {idx + 1} Déclenché', str(fired)))
                    entries.append((f'Pyro {idx + 1} Temps (ms)', str(time_ms)))
                    off += 8

                metric_names = ['Max Alt GPS', 'Max Alt Baro', 'Max Alt Kalman', 'Apogée', 'Déploiement Principal', 'Déploiement Drogue', 'Vitesse Asc Max', 'Accel Asc Max', 'Vitesse Desc Max', 'Accel Desc Max']
                for name in metric_names:
                    valid = bool(self.data[off])
                    val = float(np.frombuffer(self.data[off + 4:off + 8], dtype='<f4')[0])
                    tms = int.from_bytes(self.data[off + 8:off + 12], 'little')
                    if valid:
                        entries.append((name, f"{val:.3f} (à {tms} ms)"))
                    off += 12

                flight_time_ms = int.from_bytes(self.data[off:off + 4], 'little')
                entries.append(('Temps de vol total (ms)', str(flight_time_ms)))
        return entries

    def get_flight_packets(self, flight):
        data_start = flight['header_addr'] + self.sector_size + self.page_size
        packets = []
        off = data_start

        def f32(b, s=0): return float(np.frombuffer(b[s:s + 4], dtype='<f4')[0])
        def i32(b, s=0): return int.from_bytes(b[s:s + 4], 'little', signed=True)
        def u16(b, s=0): return int.from_bytes(b[s:s + 2], 'little', signed=False)

        while off + 4 <= len(self.data):
            if int.from_bytes(self.data[off:off + 4], 'little') != 0x44415441:
                break
            odb_off = off + 4
            if odb_off + 128 > len(self.data):
                break
            pkt = self.data[odb_off:odb_off + 128]

            packets.append({
                'Time (ms)': int.from_bytes(pkt[0:4], 'little'),
                'Battery (mV)': u16(pkt, 8),
                'Roll': f"{f32(pkt, 10):.2f}",
                'Pitch': f"{f32(pkt, 14):.2f}",
                'Yaw': f"{f32(pkt, 18):.2f}",
                'Alt MSL (m)': f"{f32(pkt, 58):.2f}",
                'Temp (°C)': f"{f32(pkt, 66):.2f}",
                'GPS Alt (m)': f"{i32(pkt, 91) / 1000.0:.2f}",
                'Vel (m/s)': f"{u16(pkt, 95) / 100.0:.2f}",
                'Kalman Z': f"{f32(pkt, 108):.2f}"
            })
            off += 132
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
