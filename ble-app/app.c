#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/l2cap.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_DATA_LENGTH 1024

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <device_address>\n", argv[0]);
  }

  char *device_address = argv[1];
  inquiry_info *devices = NULL;
  int max_devices = 5;
  int num_devices = 0;
  int device_id = hci_get_route(NULL);

  if (device_id < 0) {
    perror("Error opening HCI device");
    exit(1);
  }

  int sock = hci_open_dev(device_id);
  if (sock < 0) {
    perror("Error opening socket");
    exit(1);
  }

  if (hci_le_set_scan_parameters(sock, 0x01, htobs(0x0010), htobs(0x0010), 0x00,
                                 0x00, 1000 < 0) ||
      hci_le_set_scan_enable(sock, 0x01, 1, 1000) < 0) {
    perror("Error opening socket");
    exit(1);
  }

  inquiry_info *ii = NULL;
  num_devices =
      hci_inquiry(device_id, 8, max_devices, NULL, &devices,
                  IREQ_CACHE_FLUSH | IREQ_CACHE_FLUSH | IREQ_CACHE_FLUSH);
  if (num_devices < 0) {
    perror("Error during inquiry");
    exit(1);
  }

  if (argc != 2) {
    printf("Available Bluetooth Devices:\n");

    int i;
    for (i = 0; i < num_devices; i++) {
      char addr[19] = {0};
      ba2str(&(devices + i)->bdaddr, addr);
      printf("%d. %s\n", i + 1, addr);
    }

    exit(1);
  }

  int i;
  for (i = 0; i < num_devices; i++) {
    char addr[19] = {0};
    ba2str(&(devices + i)->bdaddr, addr);
    if (strcmp(addr, device_address) == 0) {
      ii = (devices + i);
      break;
    }
  }

  if (!ii) {
    fprintf(stderr, "Device not found\n");
    exit(1);
  }

  struct sockaddr_l2 addr = {
      .l2_family = AF_BLUETOOTH,
      .l2_psm = htobs(0x1001),
      .l2_bdaddr = ii->bdaddr,
  };

  int s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_L2CAP);
  if (s < 0) {
    perror("Error creating socket");
    exit(1);
  }

  if (connect(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("Error connecting");
    close(s);
    exit(1);
  }

  char buffer[MAX_DATA_LENGTH];
  size_t bytes_read;

  while (1) {
    bytes_read = read(s, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
      perror("Error reading from socket");
      break;
    }
    if (bytes_read == 0) {
      printf("Connection closed\n");
      break;
    }
    buffer[bytes_read] = '\0';
    printf("Received data: %s\n", buffer);
  }

  close(s);
  free(devices);
  return 0;
}
