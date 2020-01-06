#include <Arduino.h>
#include <SPI.h>

#define STD_LED 13

static const int cs_pin = 10;
static const int step_period_us = 2000;
SPISettings settings = SPISettings(500000, MSBFIRST, SPI_MODE0);

void spi_setup()
{
    pinMode(cs_pin, OUTPUT);
    digitalWrite(cs_pin, LOW);

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

uint16_t read_reg(uint8_t addr)
{
    uint16_t val = 0xFFFF;

    digitalWrite(cs_pin, HIGH);
    SPI.beginTransaction(settings);

    val = SPI.transfer16((0x8 | (addr & 0b111)) << 12);

    SPI.endTransaction();
    digitalWrite(cs_pin, LOW);

    return val;
}

void write_reg(uint8_t addr, uint16_t val)
{
    digitalWrite(cs_pin, HIGH);
    SPI.beginTransaction(settings);

    SPI.transfer16(((addr & 0b111) << 12) | (val & 0xFFF));

    SPI.endTransaction();
    digitalWrite(cs_pin, LOW);

    return val;
}

void enable_motor()
{
    uint16_t val = read_reg(0x0);
    write_reg(0x0, val | 1);
}

void step()
{
    uint16_t val = read_reg(0x0);
    write_reg(0x0, val | (1 << 2));
}

void set_torque_and_gain()
{
    uint16_t treg = read_reg(0x01);
    uint16_t creg = read_reg(0x00);

    write_reg(0x00, creg | (0b11 << 8)); // set gain to 40
    write_reg(0x01, treg | 0x6F); // brings torque down for ~1000 mA limit
}


void setup() 
{
    //pinMode(cs_pin, OUTPUT);
    spi_setup();
    serial_setup();

    delay(5000);

    pinMode(STD_LED, OUTPUT);
    digitalWrite(STD_LED, 1);

    write_reg(0x7, 0);
    set_torque_and_gain();
    enable_motor();
    
    while(1) {
        for(uint8_t i = 0; i <= 7; i++) {
            Serial.println(read_reg(i));
            delay(1);
        }
        step();
        delay(100);
      
    };
}

void loop() {}
