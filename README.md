# mbed-hx711

An MbedOS Driver for the Avia Semiconductor HX711 Load Cell Amplifier. This should be easily adaptable to other environments like Arduino.

Unlike many other open-source drivers for this chip, this driver uses an interrupt on the data pin to allow non-blocking operation, so your device can weigh things and chew gum at the same time. 

## Setup

Connect a digital output pin to the clock (`PD_SCK`) pin on the HX711, and an interrupt-capable input pin to the data (`DOUT`) pin. 

Instantiate an `HX711` with the output pin, the input pin, and a pointer to a function that will be called when a new sample has been read. The callback function should accept a signed 32-bit integer. Note that the callback function runs in an interrupt context, so it's best to either set a global variable with the value or use an event queue when calling a slow or I/O-intensive function like `printf`. 

Once the driver is instantiated, call its `start` method. Several milliseconds later your callback function should be called with a result. 

To power down the chip and stop calling your callback, call the driver's `stop` method.

If you adjust the gain value, the driver will skip calling the callback function once until the new gain value has taken effect on the next read. 

### Multiple HX711s

Multiple instances of the driver can coexist, but each must have a dedicated clock and data pin. 

Some boards have an issue where interrupts are shared across pins in a way that's either a leaky abstraction or a bug. Specifically STM NUCLEO boards will need to use different pin *numbers* even if they are on different *ports*, and pins 0 and 1 (regardless of port) did not work for me at all. 

