# Documentation

[Robot hardware documentation](https://fs.keyestudio.com/KS0470)

[Microcontroller datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)

[Arduino Uno Pinout](https://images.prismic.io/circuito/8e3a980f0f964cc539b4cbbba2654bb660db6f52_arduino-uno-pinout-diagram.png)

# Header files

[`pins.h`](include/pins.h) - a list of pin port numbers and their output direction

[`motor.h`](include/motor.h) - initialise and control the motors

[`irsensor.h`](include/irsensor.h) - initialization and functionality of IR sensors

[`pid.h`](include/pid.h) - initialization of pid controller

[`helpers.h`](include/helpers.h) - helper functions

[`bluetooth.h`](include/bluetooth.h) - initialization of bluetooth module and its functionality

[`bluetooth_commands.h`](include/bluetooth_commands.h) - command functions for the bluetooth module


# Timers

Timer 0 - left & right motor

Timer 1 - PID

Timer 2 - Unused

# Interrupts

OCIE1A - left motor

OCIE1B - right motor

# Bluetooth

Name of bluetooth module is ... (TODO)

## API

Each separate `|` represents a separate byte message

Start Robot - `'A'`

Penalty Time - `'B'`

Stop Robot - `'C'`

Start Calibration - `'D'`

Set IR Calibration - `0x06 | min (1 byte) | max (1 byte)`

Set Goal Position - `0x05` (unfinished)