#include "SerialPacket.h"

#include <unity.h>
#include <iostream>
#include <string.h>

void test_serialization(void) {
    Serial::CmdMsg to_send = {1, 1, 150, 1};
    Serial::CmdMsg rcv;
    std::vector<char> buff;
   
    Serial::pack(buff, to_send);
    Serial::unpack(buff, rcv);

    TEST_ASSERT_EQUAL(to_send.is_relative, rcv.is_relative);
    TEST_ASSERT_EQUAL(to_send.m1_angle, rcv.m1_angle);
    TEST_ASSERT_EQUAL(to_send.m2_angle, rcv.m2_angle);
    TEST_ASSERT_EQUAL(to_send.m3_angle, rcv.m3_angle);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_serialization);
    UNITY_END();
    return 0;
}
