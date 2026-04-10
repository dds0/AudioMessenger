#ifndef AUDIOCONSTANTS_H
#define AUDIOCONSTANTS_H

#include <QAudioFormat>

namespace AC
{
    enum class Codec : uint8_t {
        Pcm = 0
    };

    enum VadMode {
        Normal = 0, LowBitrate = 1, Aggressive = 2, VeryAggressive = 3
    };

    const int outgoingCallAudioFlowId = 0;
    const int incomingCallAudioFlowId = 1;
    const int dropCallAudioFlowId = 2;
    const int selfListeningAudioFlowId = 0xFE;

    const int SamplesInMs = 8;
    const int BytesInSample = 2;
    const int TargetSampleRate = 8000;
    const int BytesPerMillisecond = 16;

    const uint16_t StatusInterval = 500;
    const uint16_t MsIntervalEnableStatus = 1500;

    QAudioFormat AudioFormat();

    struct AudioFrame
    {
        Codec codec;

        uint8_t duration;
        QByteArray payload;
    };
}

#endif // AUDIOCONSTANTS_H
