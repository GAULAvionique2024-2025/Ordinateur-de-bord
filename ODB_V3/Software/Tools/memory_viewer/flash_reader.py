import os
from PyQt5.QtCore import QThread, pyqtSignal

class FlashReaderThread(QThread):
    progress = pyqtSignal(int)
    finished = pyqtSignal(bytes)
    cancelled = pyqtSignal()
    error = pyqtSignal(str)

    def __init__(self, drive):
        super().__init__()
        self.drive = drive
        self.to_read = 64 * 1024 * 1024  # Lecture de 64 MiB
        self.chunk_size = 1024 * 1024    # Morceaux de 1 MiB

    def run(self):
        if os.name == 'nt':
            dev_path = r"\\.\%s" % self.drive.replace('\\\\', '').rstrip('\\')
        else:
            dev_path = self.drive

        try:
            loaded = bytearray()
            with open(dev_path, 'rb') as f:
                remaining = self.to_read
                total = self.to_read
                cancelled = False
                while remaining > 0:
                    if self.isInterruptionRequested():
                        cancelled = True
                        break
                    chunk = f.read(min(self.chunk_size, remaining))
                    if not chunk:
                        break
                    loaded.extend(chunk)
                    remaining -= len(chunk)
                    
                    # Calculer et envoyer la progression (0 à 100%)
                    percent = int(((total - remaining) / total) * 100)
                    self.progress.emit(percent)

            if cancelled:
                self.cancelled.emit()
                return

            self.finished.emit(bytes(loaded))
        except Exception as e:
            self.error.emit(str(e))