#include "StepperMotor.h"
#include "util.h"

#include <Arduino.h>
#include <SPI.h>

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
    Serial.println("Starting...");
}

int8_t get_char() {
    if (Serial.available() > 0) return Serial.read();
    return -1;
}

void setup() 
{
    //pinMode(cs_pin, OUTPUT);
    spi_setup();
    serial_setup();

    delay(1000);

    StepperMotor sm(PIN_M1_CS); 
    delay(1);
    
    pinMode(STD_LED, OUTPUT);
    digitalWrite(STD_LED, 1);


    while(1) {
        int8_t val = -1;
        while(val == -1) {val = get_char();}

        sm.set_angle(val, 1);
        Serial.println("Moving ...");
        while(!sm.inc_steps()){};
        
        //for (int ang = 0; ang <= 360; ang += 90) {
        //    sm.set_angle(ang, 1);
        //    Serial.println("Moving forward...");
        //    while(!sm.inc_steps()){};

        //    delay(3000);
        //    Serial.println(sm.readStatus());
        //    Serial.println(sm.verifySettings());
        //}
    };
}

void loop() {}
