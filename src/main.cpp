#include <Arduino.h>
#include <HighPowerStepperDriver.h>
#include <SPI.h>

#define STD_LED 13

static const int cs_pin = 10;
static const int step_period_us = 2000;

void spi_setup()
{
    SPI.setMOSI(11);
    SPI.setMISO(12);
    SPI.setSCK(14);
    SPI.begin();
}

void serial_setup()
{
    Serial.begin(115200);

    while(!Serial){};
    Serial.println("Hello");
}

void spi_write(int addr, int val)
{
    digitalWrite(cs_pin, LOW);
    SPI.transfer(addr);
    SPI.transfer(val);
    digitalWrite(cs_pin, HIGH);
}

void setup_stepper(HighPowerStepperDriver& sd)
{
    sd.setChipSelectPin(cs_pin);
    delay(1);
    sd.resetSettings();
    delay(1);
    sd.clearStatus();
    delay(1);
    sd.setDecayMode(HPSDDecayMode::AutoMixed);
    delay(1);
    sd.setCurrentMilliamps36v4(1000);
    delay(1);
    sd.setStepMode(HPSDStepMode::MicroStep1);
    delay(1);
    sd.enableDriver();
}

void setup() 
{
    //pinMode(cs_pin, OUTPUT);
    spi_setup();
    serial_setup();

    delay(1000);

    HighPowerStepperDriver sd;
    setup_stepper(sd);
    delay(1);
    
    
    pinMode(STD_LED, OUTPUT);
    digitalWrite(STD_LED, 1);


    while(1) {
        sd.setDirection(0);
        Serial.println("Moving forward...");
        for(uint32_t i = 0; i < 1000; i++)
        {
            //spi_write(0x00, 0b101);
            sd.step();
            delayMicroseconds(step_period_us);
        }

        delay(3000);
        Serial.println(sd.readStatus());
        Serial.println(sd.verifySettings());

        Serial.println("Moving backward...");
        sd.setDirection(1);
        for(uint32_t i = 0; i < 1000; i++)
        {
            sd.step();
            delayMicroseconds(step_period_us);
        }

        delay(3000);
        

    };
}

void loop() {}
