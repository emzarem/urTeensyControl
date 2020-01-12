#include "SerialPacket.h"

#include <unity.h>

void test_serialization(void) {
    SerialPacket sp(1,10.1, -20, 30);
    char* buff = new char[100];

    SerialPacket::serialize(sp, buff);

    SerialPacket ret(0,0,0,0);
    SerialPacket::deserialize(ret, buff);

    TEST_ASSERT_EQUAL(sp.is_relative, ret.is_relative);
    TEST_ASSERT_EQUAL(sp.m1_angle, ret.m1_angle);
    TEST_ASSERT_EQUAL(sp.m2_angle, ret.m2_angle);
    TEST_ASSERT_EQUAL(sp.m3_angle, ret.m3_angle);

    delete buff;
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_serialization);
    UNITY_END();
    return 0;
}
