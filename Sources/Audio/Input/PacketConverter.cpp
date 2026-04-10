#include "PacketConverter.h"
#include "Audio/Utils/AudioConstants.h"

PacketConverter::PacketConverter(int outputAudioDuration, QObject *parent)
    : QObject(parent)
{
    setOutputAudioDuration(outputAudioDuration);
    _buffer.reserve(_frameDuration * AC::BytesPerMillisecond * 2);
}

void PacketConverter::setOutputAudioDuration(uint16_t ms)
{
    if (ms == 0) { return; }
    _frameDuration = ms;
}

int PacketConverter::getOutputAudioDuration() const
{
    return _frameDuration;
}

void PacketConverter::appendAudio(const QByteArray &audio)
{
    _buffer.append(audio);

    CheckBuffer();
}

int PacketConverter::getCurrentBufferSize() const
{
    return _buffer.size();
}

void PacketConverter::clear()
{
    _buffer.clear();
}

void PacketConverter::CheckBuffer()
{
    int bytesInFrame = _frameDuration * AC::BytesPerMillisecond;

    while (_buffer.size() >= bytesInFrame)
    {
        emit audioReady(_buffer.mid(0, bytesInFrame));
        _buffer.remove(0, bytesInFrame);
    }
}
