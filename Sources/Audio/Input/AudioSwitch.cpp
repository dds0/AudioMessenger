#include "AudioSwitch.h"

using namespace AC;

AudioSwitch::AudioSwitch(QObject *parent) :
    QObject(parent),
    _gatewayState(false)
{}

void AudioSwitch::setGateway(bool state)
{
    _gatewayState = state;
}

void AudioSwitch::appendOutgoingAudio(const QByteArray &audio)
{
    if (_gatewayState)
    {
        uint8_t ms = audio.size() / AC::BytesPerMillisecond;
        emit outgoingAudioFrameReady(AC::AudioFrame{Codec::Pcm, ms, audio});
    }
}

void AudioSwitch::appendIncomingAudioFrame(const AC::AudioFrame &frame)
{
    if (_gatewayState) {
        emit incomingAudioReady(frame.payload);
    }
}
