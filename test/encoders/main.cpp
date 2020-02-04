#include "util.h"

#include <Arduino.h>
#include <Encoder.h>

static inline void serial_setup()
{
    Serial.begin(SER_BAUD_RATE);
    while (!Serial){};
    Serial.println("");
}

void blink() {
    digitalWrite(STD_LED, 1);
    delay(100);
    digitalWrite(STD_LED, 0);
    delay(100);
    digitalWrite(STD_LED, 1);
}

void setup() 
{
    pinMode(STD_LED, OUTPUT);
    digitalWrite(STD_LED, 1);

    serial_setup();
    delay(1000);

    Encoder e1(PIN_ENC1A, PIN_ENC1B);
    Encoder e2(PIN_ENC2A, PIN_ENC2B);
    Encoder e3(PIN_ENC3A, PIN_ENC3B);

    char buffer[100];

    while(1) {
        sprintf(buffer, "Encoder 1: %ld  | Encoder 2: %ld | Encoder 3: %ld", e1.read(), e2.read(), e3.read());
        Serial.println(buffer);
        delay(500);
    };
}

void loop() {}
