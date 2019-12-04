#include <Arduino.h>
#include <SPI.h>

#define STD_LED 13

static const int cs_pin = 10;

void spi_setup()
{
    SPI.setMOSI(11);
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

void setup() 
{
    pinMode(cs_pin, OUTPUT);
    spi_setup();
    serial_setup();
    
    pinMode(STD_LED, OUTPUT);
    digitalWrite(STD_LED, 1);

    int i = 0;
    while(1) {
        delay(1000);
        spi_write(i, i);
        i++;
        Serial.println(i);
    };
}

void loop() {}
