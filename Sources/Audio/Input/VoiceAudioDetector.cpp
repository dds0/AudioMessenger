#include "VoiceAudioDetector.h"
#include <QVector>
#include <cstring>

VoiceAudioDetector::VoiceAudioDetector(QObject *parent)
    : QObject(parent)
{
    WebRtcVad_Create(&_vad);
    WebRtcVad_Init(_vad);
    setMode(AC::VadMode::VeryAggressive);
}

bool VoiceAudioDetector::setMode(AC::VadMode mode)
{
    //  0 - (OK),
    // -1 - (NULL pointer, mode could not be set or the VAD instance
    bool isSet = 1 + WebRtcVad_set_mode(_vad, static_cast<int>(mode));
    if (isSet) {
        _mode = mode;
    }
    return isSet;
}

AC::VadMode VoiceAudioDetector::getMode() const
{
    return _mode;
}

VoiceAudioDetector::~VoiceAudioDetector()
{
    WebRtcVad_Free(_vad);
}

void VoiceAudioDetector::appendAudio(const QByteArray &pcm)
{
    auto ptr = (int16_t*)pcm.constData();

    auto sampleRate = AC::TargetSampleRate;
    auto bytes = pcm.size() / AC::BytesInSample;

    bool isValid = WebRtcVad_Process(_vad, sampleRate, ptr, bytes);
    if (isValid) { emit audioReady(pcm); }
}
