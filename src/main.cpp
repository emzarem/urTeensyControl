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
}

void setup() 
{
    spi_setup();
    serial_setup();

    delay(1000);

    StepperMotor sm1(PIN_M1_CS, PIN_LIM1, HPSDDecayMode::AutoMixed, 500);
    StepperMotor sm2(PIN_M2_CS, PIN_LIM2, HPSDDecayMode::AutoMixed, 500);
    StepperMotor sm3(PIN_M3_CS, PIN_LIM3, HPSDDecayMode::AutoMixed, 500);

    pinMode(PIN_N_SLP, OUTPUT);
    pinMode(PIN_RST, OUTPUT);
    digitalWrite(PIN_N_SLP, 1);
    digitalWrite(PIN_RST, 0);

    delay(1);
    
    pinMode(STD_LED, OUTPUT);
    digitalWrite(STD_LED, 1);

    std::vector<char> bytes;

    while(1) {
        if (Serial.available() > 0) { bytes.push_back(Serial.read()); }
        
        if (bytes.size() >= sizeof(SerialUtils::CmdMsg) ) {
            SerialUtils::CmdMsg rcvd = {0};
            SerialUtils::unpack(bytes.begin(), bytes.begin() + sizeof(SerialUtils::CmdMsg), rcvd);
            bytes.erase(bytes.begin(), bytes.begin() + sizeof(SerialUtils::CmdMsg));

            sm1.set_angle(rcvd.m1_angle, !rcvd.is_relative);
            sm2.set_angle(rcvd.m2_angle, !rcvd.is_relative);
            sm3.set_angle(rcvd.m3_angle, !rcvd.is_relative);
        }
        
        sm1.inc_steps();
        sm2.inc_steps();
        sm3.inc_steps();
    };
}

void loop() {}
