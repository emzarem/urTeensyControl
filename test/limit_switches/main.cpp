#include "util.h"

#include <Arduino.h>

#define DEBUG

static inline void serial_setup()
{
    Serial.begin(SER_BAUD_RATE);
    while (!Serial){};
#ifdef DEBUG    
    Serial.println("Starting...");
#endif
}

void ISR(void) {
    Serial.println("Triggered");
}

void setup() 
{
    serial_setup();
    pinMode(PIN_LIM1, INPUT);

    delay(1);
    
    pinMode(STD_LED, OUTPUT);
    digitalWrite(STD_LED, 1);

    attachInterrupt(PIN_LIM1, ISR, FALLING);

    while(1) {
//        Serial.println(digitalRead(PIN_LIM1));
    };
}

void loop() {}
