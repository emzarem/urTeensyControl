#include "SerialPacket.h"

/* Function: Constructor
 * Inputs:
 *              None
 * Outputs:
 *              None
 */
SerialPacket::SerialPacket()
    : is_relative(false), m1_angle(0), m2_angle(0), m3_angle(0) {}

/* Function: Constructor
 * Inputs:
 *              is_relative     - relative angle or absolute
 *              m1_angle        - motor one angle
 *              m2_angle        - motor two angle
 *              m3_angle        - motor three angle
 * Outputs:
 *              None
 */
SerialPacket::SerialPacket(bool is_relative,
                           float m1_angle,
                           float m2_angle,
                           float m3_angle)
    : is_relative(is_relative),
      m1_angle(m1_angle),
      m2_angle(m2_angle),
      m3_angle(m3_angle) {}

/* Function: serialize
 * Inputs:
 *              sp      - serial packet to serialize
 *              buff    - char array to pack into
 * Outputs:
 *              buff    - by pointer
 */
void SerialPacket::serialize(SerialPacket& sp, char* buff) {
    // Load in member values
    bool* p_bool = (bool*)buff;
    *p_bool++ = sp.is_relative;

    float* p_float = (float*)p_bool;
    *p_float++ = sp.m1_angle;
    *p_float++ = sp.m2_angle;
    *p_float = sp.m3_angle;
}

/* Function: deserialize
 * Inputs:
 *              sp      - serial packet to serialize
 *              buff    - char array to pack into
 * Outputs:
 *              sp      - by reference
 */
void SerialPacket::deserialize(SerialPacket& sp, char* buff) {
    bool* p_bool = (bool*)buff;
    sp.is_relative = *p_bool++;
    
    float* p_float = (float*)p_bool;
    sp.m1_angle = *p_float++; 
    sp.m2_angle = *p_float++;
    sp.m3_angle = *p_float; 
}
