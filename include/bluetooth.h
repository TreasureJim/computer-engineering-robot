#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

void initialise_bluetooth();

void Bluetooth_SendBytes(uint8_t *data, uint8_t n);

typedef struct
{

} CalibrationDiagnostics;

typedef struct
{

} RunningDiagnostics;

#endif