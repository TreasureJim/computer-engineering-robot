import sys
import socket

def main():
    if len(sys.argv) < 2:
        print("Usage: python bluetooth_reader.py <device_address>")
        return

    device_address = sys.argv[1]

    try:
        sock = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)
        print("Connecting to device...")
        sock.connect((device_address, 1))
        print("Connected to device.")

        while True:
            data = sock.recv(1024)
            if not data:
                break
            print("Received:", data.decode("utf-8"))

    except socket.error as e:
        print("Bluetooth connection error:", e)

    finally:
        sock.close()
        print("Disconnected from device.")

if __name__ == "__main__":
    main()
