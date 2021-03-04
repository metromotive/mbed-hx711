// Copyright 2021 Metromotive

#include "HX711.hpp"

namespace metromotive {

// Creates a new HX711 object with the default gain.
HX711::HX711(DigitalOut *clockPin, InterruptIn *dataPin,
             void (*onRead)(int32_t reading))
    : clockPin(clockPin), dataPin(dataPin), onRead(onRead) {
    lastSampleGain = x128;
    gain = x128;
    this->stop();
}

// Powers up the HX711 and begins listening for readings.
void HX711::start() {
    dataPin->fall(callback(this, &HX711::read));
    clockPin->write(0);
}

// Powers down the HX711.
void HX711::stop() {
    dataPin->fall(NULL);
    clockPin->write(1);
}

void HX711::read() {
    // Data pin will fall repeatedly, so ignore it during read.
    dataPin->disable_irq();

    uint32_t count = 0;

    // Can't interrupt this section for more than 60us
    __disable_irq();

    // Cycle 24 times to read value.
    for (int i = 0; i < 24; i++) {
        clockPin->write(1);
        count = count << 1;
        wait_ns(10);
        clockPin->write(0);
        count |= dataPin->read();
        wait_ns(10);
    }

    // Cycle 1 to 3 more times to set next time's gain.
    for (int i = 0; i < gain; i++) {
        clockPin->write(1);
        wait_ns(25);
        clockPin->write(0);
    }

    __enable_irq();

    // Only report result if gain/input matches requested gain/input.
    if (lastSampleGain == gain) {
        // Convert unsigned 24-bit 2's complement to 32 bit signed.
        int32_t result = ((int32_t)(count << 8)) >> 8;

        onRead(result);
    }

    // Next read will have the gain requested during this read.
    lastSampleGain = gain;

    dataPin->enable_irq();
}

} // namespace metromotive