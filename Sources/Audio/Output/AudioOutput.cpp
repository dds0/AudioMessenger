#include <QDebug>
#include <QDateTime>

#include <cmath>
#include <cstring>
#include <algorithm>
#include "AudioOutput.h"
#include "Audio/Utils/AudioConstants.h"

AudioOutput::AudioOutput(QObject *parent)
    : QObject(parent),
      _maxNoiseLevel{},
      _volume(1)
{
    connect(&_noiseLevelSenderTimer, &QTimer::timeout, this,
    [=]() { emit noiseLevel(_maxNoiseLevel); _maxNoiseLevel = {}; });

    _noiseLevelSenderTimer.start(100); // 100ms
}

AudioOutput::~AudioOutput()
{
    if (_output.isNull()) { return; }
    _output->stop();
}

void AudioOutput::setOutputDevice(const QAudioDeviceInfo &device)
{
    if (device == _device) {
        return;
    }

    _device = device;

    if (!_output.isNull()) {
        _output->stop();
        _output->reset();
    }

    _output.reset(new QAudioOutput(device, AC::AudioFormat()));
    _output->setBufferSize(800);

    _buf = _output->start();
}

void AudioOutput::setVolume(float coefficient)
{
    coefficient = coefficient < 0 ? 0 : coefficient;
    coefficient = coefficient > 2.0 ? 2.0 : coefficient;

    _volume = coefficient;
}

float AudioOutput::getVolume() const
{
    return _volume;
}

void AudioOutput::appendAudio(const QByteArray &audio)
{
    if (_output.isNull()) { return; }

    auto error = _output->error();
    auto state = _output->state();

    bool noError = error == QAudio::NoError || error == QAudio::UnderrunError;

    if (!_output.isNull() && noError && state != QAudio::StoppedState) {
        _buf->write(updateVolume(audio));
    }
}

QByteArray AudioOutput::updateVolume(const QByteArray &audio)
{
    // I don't want to use AudioOutput::setVolume method due to my experience
    // It doens't work properly and predictable, especially on linux

    QVector<int16_t> samples(audio.size() / AC::BytesInSample, 0);
    std::memcpy(samples.data(), audio.data(), audio.size());

    int64_t sumSamples{};
    for (auto&& sample: samples)
    {
        std::clamp(sample *= _volume, int16_t(INT16_MIN), int16_t(INT16_MAX));
        sumSamples += std::abs(sample);
    }
    _maxNoiseLevel = std::max<int16_t>(_maxNoiseLevel, (sumSamples / samples.size()));

    return QByteArray((char*)samples.data(), audio.size());
}
