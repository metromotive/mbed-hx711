// Copyright 2021 Metromotive

#ifndef HX711_HX711_H
#define HX711_HX711_H

#include "mbed.h"
#include <stdint.h>

namespace metromotive {

class HX711 {
  public:
    HX711(DigitalOut *clockPin, InterruptIn *dataPin,
          void (*onRead)(int32_t reading));

    void start();
    void stop();

    // Numeric value is the number of additional clock cycles to write after
    // reading value.
    enum Gain { x128 = 1, x32, x64 };

    Gain gain;

  private:
    void read();

    DigitalOut *clockPin;
    InterruptIn *dataPin;
    Gain lastSampleGain;
    void (*onRead)(int32_t reading);
};

} // namespace metromotive

#endif // HX711_HX711_H