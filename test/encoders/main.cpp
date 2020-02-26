#include "util.h"

#include <Arduino.h>
#include <Encoder.h>

Encoder e1(PIN_ENC1A, PIN_ENC1B);
Encoder e2(PIN_ENC2A, PIN_ENC2B);
Encoder e3(PIN_ENC3A, PIN_ENC3B);

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

void isr1(void){
    if(digitalRead(PIN_LIM1) == HIGH)
        e1.write(0);
}

void isr2(void) {
    if(digitalRead(PIN_LIM2) == HIGH)
        e2.write(0);
}

inline int16_t enc_to_step(int32_t enc_val) {
    return -1*enc_val *400 / 1024;
}

void setup() 
{
    pinMode(STD_LED, OUTPUT);
    digitalWrite(STD_LED, 1);

    serial_setup();
    delay(1000);

    char buffer[100];

    attachInterrupt(PIN_LIM1, isr1, CHANGE);
    attachInterrupt(PIN_LIM2, isr2, CHANGE);

    while(1) {
        sprintf(buffer, "Encoder 1: %ld  | Encoder 2: %ld | Encoder 3: %ld", enc_to_step(e1.read()), enc_to_step(e2.read()), enc_to_step(e3.read()));
        Serial.println(buffer);
        delay(500);
    };
}

void loop() {}
