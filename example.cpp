#include "mbed.h"
#include "HX711.hpp"

using namespace metromotive;

DigitalOut hx711Clock(PB_4);
InterruptIn hx711Interrupt(PB_3);

volatile int32_t rawReading = 0;

void onHX711Read(int32_t result) { rawReading = result; }

HX711 hx711(&hx711Clock, &hx711Interrupt, &onHX711Read);

int main() {
    hx711.start();
    
    while (true) {
        printf("Sensor raw reading is %d.\n", rawReading);
        
        ThisThread::sleep_for(100ms);
    }
}

