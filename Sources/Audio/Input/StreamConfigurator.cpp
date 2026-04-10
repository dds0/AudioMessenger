#include "StreamConfigurator.h"
#include "CS_Common/Common/MachineTypes.h"

StreamConfigurator::StreamConfigurator(QObject *parent)
    : QObject(parent), _channel(CommonUndefinedId) {}

void StreamConfigurator::SetProto(QWeakPointer<DSP> proto)
{
    if (proto.isNull()) { return; }
    _proto = proto;
}

void StreamConfigurator::SetMainChannel(uint8_t channel)
{
    _channel = channel;
}

uint8_t StreamConfigurator::GetChannel() const
{
    return _channel;
}

void StreamConfigurator::SendFrame(const AudioFrame &frame)
{
    message_VOICE msg;

    msg.codec = ConvertCodecType(frame.codec);
    msg.dataLen = frame.payload.size();
    std::copy_n(frame.payload.data(),
        frame.payload.size(), std::back_inserter(msg.payload));
    msg.duration = static_cast<enum_FrameDuration>(frame.duration);
    msg.ch_number = _channel;

    if (!_proto.isNull()) {
        _proto.lock().get()->packet_voice_s(&msg);
    }
}

void StreamConfigurator::SendFrames(const std::vector<AudioFrame> &frames)
{
    for (const auto& frame: frames) {
        SendFrame(frame);
    }
}

enum_Codec StreamConfigurator::ConvertCodecType(CodecType type)
{
    switch (type) {
    case CodecType::pcm: return enum_Codec::pcm;
    case CodecType::opus: return enum_Codec::opus;
    //case CodecType::amr_nb: return enum_Codec::amr_nb;
    default:
        return enum_Codec::pcm;
    }
}
