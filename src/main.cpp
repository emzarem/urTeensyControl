#include "SerialPacket.h"
#include "StepperMotor.h"
#include "util.h"

#include <Arduino.h>
#include <SPI.h>
#include <vector>

//#define USE_USB

#ifdef USE_USB
    usb_serial_class& SerialPort = Serial;
#else
    HardwareSerial& SerialPort = Serial1;
#endif

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

    StepperMotor sm1(PIN_M1_CS, PIN_LIM1, HPSDDecayMode::AutoMixed, 1000, HPSDStepMode::MicroStep4);
    StepperMotor sm2(PIN_M2_CS, PIN_LIM2, HPSDDecayMode::AutoMixed, 1000, HPSDStepMode::MicroStep4);
    StepperMotor sm3(PIN_M3_CS, PIN_LIM3, HPSDDecayMode::AutoMixed, 1000, HPSDStepMode::MicroStep4);

    pinMode(PIN_N_SLP, OUTPUT);
    pinMode(PIN_RST, OUTPUT);
    digitalWrite(PIN_N_SLP, 1);
    digitalWrite(PIN_RST, 0);

    delay(1);
    
    pinMode(STD_LED, OUTPUT);
    digitalWrite(STD_LED, 1);

    std::vector<char> bytes;
    SerialUtils::CmdMsg *p_rx_msg = NULL;

    std::vector<StepperMotor*> motors;
    motors.push_back(&sm1);
    motors.push_back(&sm2);
    motors.push_back(&sm3);

    StepperMotor::calibrate(motors);
    
    serial_setup();
    delay(1000);

    bool msg_sent = false;

    while(1) {
        if (SerialPort.available() > 0) {
            uint8_t next_byte = SerialPort.read();
            if (next_byte == SerialUtils::DELIMITER) {
                p_rx_msg = new SerialUtils::CmdMsg();
                SerialUtils::unpack(bytes, *p_rx_msg);
                bytes.clear();
            } else {
                bytes.push_back(next_byte);
            }
        }
        
        if (p_rx_msg) {
            sm1.set_angle(p_rx_msg->m1_angle, !p_rx_msg->is_relative);
            sm2.set_angle(p_rx_msg->m2_angle, !p_rx_msg->is_relative);
            sm3.set_angle(p_rx_msg->m3_angle, !p_rx_msg->is_relative);
            delete p_rx_msg;
            p_rx_msg = NULL;
            msg_sent = false;
        }
       
        bool done = true;
        for (auto& mtr : motors)
            done &= mtr->inc_steps();

        // Let the controller know if movement completed
        if (done && !msg_sent) {
            SerialUtils::CmdMsg tosend = {0};
            tosend.motors_done = true;
            std::vector<char> tx_buf;
            SerialUtils::pack(tx_buf, tosend);
            SerialPort.write((char *)&tx_buf[0], tx_buf.size());
            msg_sent = true;
        }
    };
}

void loop() {}
