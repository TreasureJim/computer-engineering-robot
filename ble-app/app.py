import sys
from bluepy.btle import Scanner, DefaultDelegate, Peripheral

class ScanDelegate(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)

    def handleDiscovery(self, dev, isNewDev, isNewData):
        if isNewDev:
            print("Discovered device:", dev.addr)

class BLEDevice:
    def __init__(self, device_address):
        self.device_address = device_address
        self.peripheral = None

    def connect(self):
        try:
            self.peripheral = Peripheral(self.device_address)
            print("Connected to device:", self.device_address)
        except Exception as e:
            print("Error connecting to device:", e)
            sys.exit(1)

    def disconnect(self):
        if self.peripheral:
            self.peripheral.disconnect()
            print("Disconnected from device:", self.device_address)

    def read_characteristic(self, service_uuid, characteristic_uuid):
        try:
            service = self.peripheral.getServiceByUUID(service_uuid)
            characteristic = service.getCharacteristics(characteristic_uuid)[0]
            value = characteristic.read()
            value_hex = value.hex()
            print("Read value:", value_hex)
            return value_hex
        except Exception as e:
            print("Error reading characteristic:", e)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        # If no address is provided, print available device addresses and exit
        scanner = Scanner().withDelegate(ScanDelegate())
        devices = scanner.scan(10.0)  # Scan for 10 seconds

        if not devices:
            print("No BLE devices found.")
            sys.exit()

        print("Available BLE devices:")
        for dev in devices:
            print("  Device:", dev.addr)
        sys.exit()

    device_address = sys.argv[1]

    # Replace with your UUIDs
    service_uuid = "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    characteristic_uuid = "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"

    # Create BLEDevice instance and connect
    device = BLEDevice(device_address)
    device.connect()

    # Read characteristic value
    value = device.read_characteristic(service_uuid, characteristic_uuid)

    # Disconnect from device
    device.disconnect()
