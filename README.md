# Documentation

[Robot hardware documentation](https://fs.keyestudio.com/KS0470)
[Microcontroller datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)
[Arduino Uno Pinout](https://images.prismic.io/circuito/8e3a980f0f964cc539b4cbbba2654bb660db6f52_arduino-uno-pinout-diagram.png)

# Header files

[`pins.h`](include/pins.h) - a list of pin port numbers and their output direction

[`motor.h`](include/motor.h) - initialise and control the motors

# Timers

Timer 0 - left & right motor

Timer 1 - Unused

Timer 2 - Unused

# Interrupts

OCIE1A - left motor

OCIE1B - right motor