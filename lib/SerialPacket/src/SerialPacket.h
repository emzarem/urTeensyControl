#pragma once

#include <vector>
#include <string>

namespace Serial {

struct CmdMsg{
    bool is_relative;
    float m1_angle, m2_angle, m3_angle;
};

union Packet {
    CmdMsg msg;
    char bytes[sizeof(CmdMsg)];
} __attribute__((packed));

inline void pack(std::vector<char> &dest, CmdMsg &src) {
    Packet pkt = { .msg=src };
    dest.insert(dest.end(), pkt.bytes, pkt.bytes + sizeof(CmdMsg));
}

inline void unpack(std::vector<char> &src, CmdMsg &dest) {
    Packet pkt;
    std::copy(src.begin(), src.end(), pkt.bytes);
    dest = pkt.msg;
}

}
