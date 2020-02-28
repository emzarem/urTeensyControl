#include "SerialPacket.h"
#include "AccelMotor.h"
#include "util.h"

#include <Arduino.h>
#include <SPI.h>
#include <vector>

#define USE_USB

#ifdef USE_USB
    usb_serial_class& SerialPort = Serial;
#else
    HardwareSerial& SerialPort = Serial1;
#endif

static const int CURRENT_LIMIT = 2500;
static const HPSDStepMode STEP_MODE = HPSDStepMode::MicroStep2;

static inline void spi_setup()
{
    SPI.setMOSI(PIN_MOSI);
    SPI.setMISO(PIN_MISO);
    SPI.setSCK(PIN_SCK);
    SPI.begin();
}

static inline void serial_setup()
{
    SerialPort.begin(SER_BAUD_RATE);
    while (!SerialPort){};
    SerialPort.println("");
}

int8_t get_char() {
    if (SerialPort.available() > 0) return SerialPort.read();
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
    delay(1000);

    AccelMotor sm1(PIN_M1_CS, PIN_LIM1, PIN_ENC1A, PIN_ENC1B, HPSDDecayMode::AutoMixed, CURRENT_LIMIT, STEP_MODE);
//    AccelMotor sm2(PIN_M2_CS, PIN_LIM2, PIN_ENC2A, PIN_ENC2B, HPSDDecayMode::AutoMixed, CURRENT_LIMIT, STEP_MODE);
//    AccelMotor sm3(PIN_M3_CS, PIN_LIM3, PIN_ENC3A, PIN_ENC3B, HPSDDecayMode::AutoMixed, CURRENT_LIMIT, STEP_MODE);

    pinMode(PIN_N_SLP, OUTPUT);
    pinMode(PIN_RST, OUTPUT);
    digitalWrite(PIN_N_SLP, 1);
    digitalWrite(PIN_RST, 0);

    delay(1);
    
    pinMode(STD_LED, OUTPUT);
    digitalWrite(STD_LED, 1);

    std::vector<char> bytes;
    SerialUtils::CmdMsg *p_rx_msg = NULL;

    std::vector<AccelMotor*> motors;
    motors.push_back(&sm1);
//    motors.push_back(&sm2);
//    motors.push_back(&sm3);

    for (auto &mtr : motors)
        mtr.setAcceleration

    AccelMotor::calibrate(motors);
    
    serial_setup();
    delay(1000);

    bool msg_sent = false;

    while(1) {
        for (auto &mtr : motors)
            mtr.set_angle(80, 1);

        while(sm1.run()) {};

        blink();
        delay(1000);
  
        for (auto &mtr : motors)
            mtr.set_angle(0, 1);

        while(sm1.run()) {};

        blink();
        delay(1000);
       
        
               
    };
}

void loop() {}
