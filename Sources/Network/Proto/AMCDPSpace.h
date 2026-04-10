#ifndef AMCPSPACE_H
#define AMCPSPACE_H

#include <stdint.h>
#include <QByteArray>
#include "Audio/Utils/AudioConstants.h"

/// Audio messenger control protocol space

namespace AMCDPSPace
{
    inline uint32_t prefix = 0xFEA0DDCC;

    enum class TypeCommand : uint8_t {
        Request = 0, Reply = 1, Status = 2, ReqNoReply = 3
    };
    enum class CodeCommand : uint8_t {
        Status = 0, StartCall = 1, EndCall = 2, DenyCall = 3, Voice = 4
    };

    #pragma pack(push, 1)
    struct Header
    {
        uint32_t protoId = prefix;
        uint8_t deviceId;

        TypeCommand type;
        CodeCommand command;
    };
    #pragma pack(pop)

    // Network commands below

    using StartCall = Header;
    using EndCall = Header;
    using DenyCall = Header;
    using ActivityStatus = Header;

    using Voice = AC::AudioFrame;
}

#endif // AMCPSPACE_H
