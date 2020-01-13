#include "SerialPacket.h"

#include <Arduino.h>
#include <vector>

#define STD_LED 13

SerialUtils::CmdMsg expected = {0, 1, 1, 1};

void serial_setup() {
    Serial.begin(115200);

    while (!Serial) {};
    Serial.println("Hello");
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

void setup() {
    serial_setup();

    pinMode(STD_LED, OUTPUT);
    digitalWrite(STD_LED, 1);

    std::vector<char> bytes;

    while (1) {
        bytes.clear();
        while (Serial.available() > 0) { bytes.push_back(Serial.read()); }
        SerialUtils::CmdMsg rcvd = {0};
        SerialUtils::unpack(bytes, rcvd);

        if (rcvd.is_relative == expected.is_relative &&
            rcvd.m1_angle == expected.m1_angle &&
            rcvd.m2_angle == expected.m2_angle &&
            rcvd.m3_angle == expected.m3_angle)
            blink();
    };
}

void loop() {}
