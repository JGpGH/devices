from deviceWatch import SerialDeviceWatcher
from flask import Flask

if __name__ == "__main__":
    watcher = SerialDeviceWatcher(
        on_create=lambda device: print(f"Device added: {device}"),
        on_delete=lambda device: print(f"Device removed: {device}")
    )
    watcher.start()
    try:
        while True:
            pass  # Keep the script running to watch for device changes
    except KeyboardInterrupt:
        print("Exiting...")
        watcher.stop()