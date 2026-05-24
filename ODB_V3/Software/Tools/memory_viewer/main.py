import sys
import os
import psutil
from PyQt5 import QtWidgets, QtCore, QtGui
from PyQt5.QtWebEngineWidgets import QWebEngineView
from flash_reader import FlashReaderThread
from data_parser import W25QParser

def list_drives():
    return [p.device for p in psutil.disk_partitions(all=False)]

class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle('Analyseur W25Q STM32')
        self.resize(1100, 700)
        self.parser = None

        # Charger le style
        try:
            with open("style.qss", "r") as f:
                self.setStyleSheet(f.read())
        except:
            print("Fichier style.qss introuvable, utilisation du style par défaut.")

        central = QtWidgets.QWidget()
        self.setCentralWidget(central)
        main_layout = QtWidgets.QVBoxLayout(central)

        # -- BARRE SUPÉRIEURE --
        top_bar = QtWidgets.QHBoxLayout()
        self.drive_combo = QtWidgets.QComboBox()
        self.drive_combo.addItems(list_drives())
        
        self.refresh_btn = QtWidgets.QPushButton('🔄 Rafraîchir Ports')
        self.read_btn = QtWidgets.QPushButton('📥 Lire la Mémoire')
        self.cancel_btn = QtWidgets.QPushButton('❌ Annuler')
        self.cancel_btn.setEnabled(False)
        self.cancel_btn.setVisible(False)
        
        self.progress_bar = QtWidgets.QProgressBar()
        self.progress_bar.setValue(0)
        self.progress_bar.setVisible(False)

        top_bar.addWidget(QtWidgets.QLabel("Port USB :"))
        top_bar.addWidget(self.drive_combo)
        top_bar.addWidget(self.refresh_btn)
        top_bar.addWidget(self.read_btn)
        top_bar.addWidget(self.cancel_btn)
        top_bar.addWidget(self.progress_bar, stretch=1)
        
        main_layout.addLayout(top_bar)

        # -- ONGLETS --
        self.tabs = QtWidgets.QTabWidget()
        main_layout.addWidget(self.tabs)

        self.tab_dashboard = QtWidgets.QWidget()
        self.setup_dashboard_tab()
        self.tabs.addTab(self.tab_dashboard, "📊 Visualisation & Espace")

        self.tab_flights = QtWidgets.QWidget()
        self.setup_flights_tab()
        self.tabs.addTab(self.tab_flights, "🚀 Vols & Données")

        self.refresh_btn.clicked.connect(self.on_refresh)
        self.read_btn.clicked.connect(self.start_reading)
        self.cancel_btn.clicked.connect(self.cancel_reading)
        self.export_btn.clicked.connect(self.export_current_flight_csv)
        self.flight_list.itemSelectionChanged.connect(self.on_select_flight)

    def setup_dashboard_tab(self):
        layout = QtWidgets.QHBoxLayout(self.tab_dashboard)
        
        # Gauche: Config
        config_group = QtWidgets.QGroupBox("Configuration Enregistrée")
        config_layout = QtWidgets.QVBoxLayout(config_group)
        self.config_table = QtWidgets.QTableWidget(0, 2)
        self.config_table.setHorizontalHeaderLabels(['Paramètre', 'Valeur'])
        self.config_table.horizontalHeader().setStretchLastSection(True)
        self.config_table.setEditTriggers(QtWidgets.QAbstractItemView.NoEditTriggers)
        config_layout.addWidget(self.config_table)
        layout.addWidget(config_group, stretch=1)

        # Droite: Graphique WebEngine Sunburst
        mem_group = QtWidgets.QGroupBox("Occupation de la Mémoire Flash")
        mem_layout = QtWidgets.QVBoxLayout(mem_group)
        
        # Initialisation du navigateur web intégré
        self.web_view = QWebEngineView()
        self.web_view.setHtml("<body style='background-color: #181825; color: white; display: flex; align-items: center; justify-content: center; font-family: sans-serif;'>Veuillez lire la mémoire pour afficher le graphique.</body>")
        
        mem_layout.addWidget(self.web_view)
        self.memory_info_label = QtWidgets.QLabel("")
        self.memory_info_label.setWordWrap(True)
        self.memory_info_label.setTextInteractionFlags(QtCore.Qt.TextSelectableByMouse)
        mem_layout.addWidget(self.memory_info_label)
        layout.addWidget(mem_group, stretch=2)

    def setup_flights_tab(self):
        layout = QtWidgets.QHBoxLayout(self.tab_flights)
        
        left_layout = QtWidgets.QVBoxLayout()
        left_layout.addWidget(QtWidgets.QLabel("Liste des Vols détectés :"))
        self.flight_list = QtWidgets.QListWidget()
        self.flight_list.setMaximumWidth(250)
        left_layout.addWidget(self.flight_list)
        self.export_btn = QtWidgets.QPushButton('⬇️ Export CSV')
        self.export_btn.setEnabled(False)
        left_layout.addWidget(self.export_btn)
        layout.addLayout(left_layout)

        self.flight_tabs = QtWidgets.QTabWidget()
        
        self.stats_table = QtWidgets.QTableWidget(0, 2)
        self.stats_table.setHorizontalHeaderLabels(['Métrique', 'Valeur'])
        self.stats_table.horizontalHeader().setStretchLastSection(True)
        self.flight_tabs.addTab(self.stats_table, "📈 Statistiques")

        self.packet_table = QtWidgets.QTableWidget()
        self.flight_tabs.addTab(self.packet_table, "📡 Packets (Télémetrie)")

        layout.addWidget(self.flight_tabs, stretch=1)

    def on_refresh(self):
        self.drive_combo.clear()
        self.drive_combo.addItems(list_drives())

    def start_reading(self):
        drive = self.drive_combo.currentText()
        if not drive:
            QtWidgets.QMessageBox.warning(self, 'Erreur', 'Sélectionnez un port.')
            return

        self.read_btn.setEnabled(False)
        self.cancel_btn.setEnabled(True)
        self.cancel_btn.setVisible(True)
        self.progress_bar.setVisible(True)
        self.progress_bar.setValue(0)

        self.reader = FlashReaderThread(drive)
        self.reader.progress.connect(self.progress_bar.setValue)
        self.reader.finished.connect(self.on_read_finished)
        self.reader.cancelled.connect(self.on_read_cancelled)
        self.reader.error.connect(self.on_read_error)
        self.reader.start()

    def cancel_reading(self):
        if hasattr(self, 'reader') and self.reader and self.reader.isRunning():
            self.cancel_btn.setEnabled(False)
            self.reader.requestInterruption()

    def _reset_reading_controls(self):
        self.read_btn.setEnabled(True)
        self.cancel_btn.setEnabled(False)
        self.cancel_btn.setVisible(False)
        self.progress_bar.setVisible(False)

    def on_read_error(self, err):
        self._reset_reading_controls()
        QtWidgets.QMessageBox.critical(self, 'Erreur de lecture', f"Impossible de lire: {err}")

    def on_read_cancelled(self):
        self._reset_reading_controls()
        QtWidgets.QMessageBox.information(self, 'Lecture annulée', 'La lecture de la mémoire a été interrompue.')

    def on_read_finished(self, data):
        self._reset_reading_controls()
        
        self.parser = W25QParser(data)
        self.parser.scan_memory()
        
        self.update_ui_after_scan()
        self.export_btn.setEnabled(bool(self.parser.flights))

    def update_ui_after_scan(self):
        self.config_table.setRowCount(len(self.parser.config_details))
        for i, (k, v) in enumerate(self.parser.config_details):
            self.config_table.setItem(i, 0, QtWidgets.QTableWidgetItem(k))
            self.config_table.setItem(i, 1, QtWidgets.QTableWidgetItem(v))

        self.flight_list.clear()
        for f in self.parser.flights:
            self.flight_list.addItem(f"Vol #{f['id']} (0x{f['header_addr']:X})")

        # Mise à jour du graphique dynamique
        self.draw_sunburst_chart()

        if self.parser.flights:
            self.flight_list.setCurrentRow(0)

    def draw_sunburst_chart(self):
        # 1. Récupération des données en octets
        summary = self.parser.get_memory_summary()
        total_size = len(self.parser.data)
        empty_size = summary['empty']
        used_size = total_size - empty_size
        config_size = summary['config']
        data_size = summary['data']
        stats_size = summary['stats']
        header_size = summary['header']
        other_used_size = summary['used']
        total_vols = header_size + stats_size + data_size
        total_mb = total_size / (1024 * 1024)
        used_percent = (used_size / total_size * 100) if total_size else 0
        config_percent = (config_size / total_size * 100) if total_size else 0
        data_percent = (data_size / total_size * 100) if total_size else 0
        stats_percent = (stats_size / total_size * 100) if total_size else 0
        remaining_pages = empty_size // self.parser.page_size

        def fmt_bytes(value):
            return f"{value:,}".replace(",", " ")

        self.memory_info_label.setText(
            "<b>Occupation:</b> "
            f"{fmt_bytes(used_size)} B ({used_percent:.3f}%) | "
            "<b>Config:</b> "
            f"{fmt_bytes(config_size)} B ({config_percent:.3f}%) | "
            "<b>Data:</b> "
            f"{fmt_bytes(data_size)} B ({data_percent:.3f}%) | "
            "<b>Stats:</b> "
            f"{fmt_bytes(stats_size)} B ({stats_percent:.3f}%) | "
            "<b>Pages restantes:</b> "
            f"{remaining_pages} | "
            "<b>Headers:</b> "
            f"{fmt_bytes(header_size)} B | "
            "<b>Autres utilisés:</b> "
            f"{fmt_bytes(other_used_size)} B"
        )

        # 2. Génération du HTML (Note le src="echarts.min.js" qui est maintenant local)
        html = f"""
        <!DOCTYPE html>
        <html>
        <head>
            <meta charset="utf-8">
            <script src="echarts.min.js"></script>
            <style>
                body {{
                    margin: 0;
                    background-color: #181825; 
                    display: flex;
                    justify-content: center;
                    align-items: center;
                    height: 100vh;
                    overflow: hidden;
                }}
                #chart-container {{
                    width: 100%;
                    height: 100%;
                }}
            </style>
        </head>
        <body>
            <div id="chart-container"></div>
            <script>
                var chartDom = document.getElementById('chart-container');
                var myChart = echarts.init(chartDom);
                
                var option = {{
                    tooltip: {{
                        formatter: function (info) {{
                            var mb = (info.value / (1024 * 1024)).toFixed(2);
                            return '<b>' + info.name + '</b><br/>Taille : ' + mb + ' MB';
                        }}
                    }},
                    title: {{
                        text: '{total_mb:.1f} MB\\nTotal',
                        left: 'center',
                        top: 'center',
                        textStyle: {{ color: '#cdd6f4', fontSize: 16, fontWeight: 'normal' }}
                    }},
                    series: {{
                        type: 'sunburst',
                        radius: ['35%', '90%'],
                        itemStyle: {{ borderWidth: 2, borderColor: '#181825', borderRadius: 4 }},
                        label: {{ show: true, color: '#11111b', fontWeight: 'bold' }},
                        data: [
                            {{
                                name: 'Vide',
                                value: {summary['empty']},
                                itemStyle: {{ color: '#313244' }},
                                label: {{ show: false }}
                            }},
                            {{
                                name: 'Vols',
                                value: {total_vols},
                                itemStyle: {{ color: '#89b4fa' }},
                                children: [
                                    {{ name: 'Data', value: {summary['data']}, itemStyle: {{ color: '#89b4fa' }} }},
                                    {{ name: 'Stats', value: {summary['stats']}, itemStyle: {{ color: '#fab387' }} }},
                                    {{ name: 'Headers', value: {summary['header']}, itemStyle: {{ color: '#f9e2af' }} }}
                                ]
                            }},
                            {{
                                name: 'Config',
                                value: {summary['config']},
                                itemStyle: {{ color: '#cba6f7' }}
                            }}
                        ]
                    }}
                }};

                myChart.setOption(option);
                window.addEventListener('resize', function() {{ myChart.resize(); }});
            </script>
        </body>
        </html>
        """
        
        # 3. Dire à PyQt5 où chercher les fichiers locaux (baseUrl)
        current_dir = os.path.dirname(os.path.abspath(__file__))
        base_url = QtCore.QUrl.fromLocalFile(current_dir + "/")
        
        # Injection du code avec l'URL de base locale
        self.web_view.setHtml(html, base_url)

    def on_select_flight(self):
        if not self.flight_list.selectedIndexes() or not self.parser: return
        idx = self.flight_list.selectedIndexes()[0].row()
        flight = self.parser.flights[idx]

        stats = self.parser.get_flight_stats(flight)
        self.stats_table.setRowCount(len(stats))
        for i, (k, v) in enumerate(stats):
            self.stats_table.setItem(i, 0, QtWidgets.QTableWidgetItem(k))
            self.stats_table.setItem(i, 1, QtWidgets.QTableWidgetItem(v))

        packets = self.parser.get_flight_packets(flight)
        if packets:
            headers = list(packets[0].keys())
            self.packet_table.setColumnCount(len(headers))
            self.packet_table.setHorizontalHeaderLabels(headers)
            self.packet_table.setRowCount(len(packets))
            
            for row_idx, pkt in enumerate(packets):
                for col_idx, key in enumerate(headers):
                    self.packet_table.setItem(row_idx, col_idx, QtWidgets.QTableWidgetItem(str(pkt[key])))
            self.packet_table.resizeColumnsToContents()

    def _get_selected_flight(self):
        if not self.parser or not self.parser.flights:
            return None

        selected_indexes = self.flight_list.selectedIndexes()
        if selected_indexes:
            flight_index = selected_indexes[0].row()
            if 0 <= flight_index < len(self.parser.flights):
                return self.parser.flights[flight_index]

        current_row = self.flight_list.currentRow()
        if 0 <= current_row < len(self.parser.flights):
            return self.parser.flights[current_row]

        return self.parser.flights[0]

    def export_current_flight_csv(self):
        if not self.parser or not self.parser.flights:
            QtWidgets.QMessageBox.warning(self, 'Export CSV', 'Aucun vol disponible à exporter.')
            return

        flight = self._get_selected_flight()
        if not flight:
            QtWidgets.QMessageBox.warning(self, 'Export CSV', 'Sélectionnez un vol à exporter.')
            return

        default_name = f"vol_{flight['id']}_telemetrie.csv"
        file_path, _ = QtWidgets.QFileDialog.getSaveFileName(
            self,
            'Exporter le vol en CSV',
            default_name,
            'CSV (*.csv)'
        )
        if not file_path:
            return

        try:
            packet_count = self.parser.export_flight_packets_csv(flight, file_path)
        except Exception as e:
            QtWidgets.QMessageBox.critical(self, 'Export CSV', f"Impossible d'exporter le CSV: {e}")
            return

        if packet_count == 0:
            QtWidgets.QMessageBox.information(self, 'Export CSV', 'Aucune donnée de télémétrie à exporter pour ce vol.')
        else:
            QtWidgets.QMessageBox.information(
                self,
                'Export CSV',
                f'Export terminé: {packet_count} lignes écrites.'
            )

if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    w = MainWindow()
    w.show()
    sys.exit(app.exec_())

# TODO: Graphiques de Télémetrie (Courbes), Filtres et Recherche
