#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

void Bluetooth_Initialise();

void Bluetooth_Send(void *data, uint8_t n);
void Bluetooth_SendIntValues(void *data, uint8_t n);
void Bluetooth_SendFloatValues(void *data, uint8_t n);
typedef struct
{

} CalibrationDiagnostics;

typedef struct
{

} RunningDiagnostics;

#endif