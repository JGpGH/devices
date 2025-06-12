import glob
import pyudev
import threading

class SerialDeviceWatcher:
    def __init__(self, on_create=None, on_delete=None):
        self.context = pyudev.Context()
        self.monitor = pyudev.Monitor.from_netlink(self.context)
        self.monitor.filter_by('tty')  # Only watch serial devices
        self.on_create = on_create
        self.on_delete = on_delete
        self._stop_event = threading.Event()

    def _watch(self):
        while not self._stop_event.is_set():
            device = self.monitor.poll(timeout=1)
            if device is None:
                continue
            if device.device_node and (
                device.device_node.startswith("/dev/ttyUSB")
                or device.device_node.startswith("/dev/ttyACM")
            ):
                if device.action == "add" and self.on_create:
                    self.on_create(device.device_node)
                elif device.action == "remove" and self.on_delete:
                    self.on_delete(device.device_node)

    def list_current(self):
        return sorted(glob.glob("/dev/ttyUSB*") + glob.glob("/dev/ttyACM*"))

    def start(self):
        self._stop_event.clear()
        self.thread = threading.Thread(target=self._watch, daemon=True)
        self.thread.start()

    def stop(self):
        self._stop_event.set()
        if self.thread.is_alive():
            self.thread.join(timeout=2)