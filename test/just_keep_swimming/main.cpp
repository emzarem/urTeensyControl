#include "SerialPacket.h"
#include "StepperMotor.h"
#include "util.h"

#include <Arduino.h>
#include <SPI.h>
#include <vector>

#define DEBUG

static inline void spi_setup()
{
    SPI.setMOSI(PIN_MOSI);
    SPI.setMISO(PIN_MISO);
    SPI.setSCK(PIN_SCK);
    SPI.begin();
}

static inline void serial_setup()
{
    Serial.begin(SER_BAUD_RATE);
    while (!Serial){};
#ifdef DEBUG    
    Serial.println("Starting...");
#endif
}

int8_t get_char() {
    if (Serial.available() > 0) return Serial.read();
    return -1;
}

void blink() {
    digitalWrite(STD_LED, 1);
    delay(100);
    digitalWrite(STD_LED, 0);
    delay(100);
    digitalWrite(STD_LED, 1);
    //    delay(250);
    //    digitalWrite(STD_LED, 0);
    //    delay(250);
    //    digitalWrite(STD_LED, 1);
}

void setup() 
{
    spi_setup();

    delay(1000);

    StepperMotor sm1(PIN_M1_CS, HPSDDecayMode::AutoMixed, 500); 
    StepperMotor sm2(PIN_M2_CS, HPSDDecayMode::AutoMixed, 500); 
    StepperMotor sm3(PIN_M3_CS, HPSDDecayMode::AutoMixed, 500); 
  
    delay(1);
    
    pinMode(STD_LED, OUTPUT);
    digitalWrite(STD_LED, 1);

    std::vector<char> bytes;

    while(1) {
        sm1.set_angle(180, 0);
        sm2.set_angle(180, 0);
        sm3.set_angle(180, 0);
        sm1.inc_steps();
        sm2.inc_steps();
        sm3.inc_steps();
    };
}

void loop() {}
