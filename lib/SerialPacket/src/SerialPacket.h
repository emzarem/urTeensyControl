#pragma once

class SerialPacket {
public:
    SerialPacket();  // Constructor
    SerialPacket(bool is_relative,
                 float m1_angle,
                 float m2_angle,
                 float m3_angle);
    ~SerialPacket() {};  // Destructor

    static void serialize(SerialPacket& sp, char* buff);
    static void deserialize(SerialPacket& sp, char* buff);

    bool is_relative;
    float m1_angle, m2_angle, m3_angle;
};
