#include "SerialPacket.h"

#include <unity.h>
#include <iostream>
#include <string.h>

void test_serialization(void) {
    SerialUtils::CmdMsg to_send = {
        .cmd_type = SerialUtils::CMDTYPE_MTRS,
        .is_relative = false,
        .mtr_angles = {15, 20, 30}
    };
    SerialUtils::CmdMsg rcv;
    std::vector<char> buff;
   
    SerialUtils::pack(buff, to_send);
    SerialUtils::unpack(buff, rcv);

    TEST_ASSERT(to_send == rcv);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_serialization);
    UNITY_END();
    return 0;
}
