#include <Arduino.h>

#define STD_LED 13

void serial_setup()
{
    Serial.begin(115200);

    while(!Serial){};
    Serial.println("Hello");
}

void blink()
{
    digitalWrite(STD_LED, 1);
    delay(250);
    digitalWrite(STD_LED, 0);
    delay(250);
    digitalWrite(STD_LED, 1);
//    delay(250);
//    digitalWrite(STD_LED, 0);
//    delay(250);
//    digitalWrite(STD_LED, 1);
}

void setup() 
{
    serial_setup();
  
    pinMode(STD_LED, OUTPUT);
    digitalWrite(STD_LED, 1);


    while(1) {
        while(Serial.available() > 0) {
            Serial.read();
            blink();
        }
    };
}

void loop() {}
