#include "AudioInput.h"
#include "PacketConverter.h"
#include "Audio/Utils/AudioConstants.h"

#include <QTimer>
#include <QDebug>
#include <QDateTime>
#include <QAudioDeviceInfo>

AudioInput::AudioInput(QObject *parent) :
    QObject(parent),
    _buf(nullptr),
    _converter(new PacketConverter(10, this))
{
    connect(_converter, &PacketConverter::audioReady, this, &AudioInput::audioReady);
}

AudioInput::~AudioInput()
{
    if (!_input.isNull()) {
        _input->stop();
    }
}

void AudioInput::setFrameSize(uint8_t ms)
{
    _converter->setOutputAudioDuration(ms);
}

void AudioInput::setInputDevice(const QAudioDeviceInfo &device)
{
    if (device == _device) {
        return;
    }

    _device = device;

    if (!_input.isNull()) {
        _input->stop();
    }

    _input.reset(new QAudioInput(device, AC::AudioFormat()));
    _input->setBufferSize(350);

    _buf = _input->start();

    QTimer::singleShot(500, Qt::TimerType::CoarseTimer, _buf, [=]()
    {
        clear();
        connect(_buf, &QIODevice::readyRead, this, &AudioInput::readAudio);
    });
}

void AudioInput::clear()
{
    auto error = _input->error();
    auto state = _input->state();
    bool expectedCondition = error == QAudio::NoError && state != QAudio::StoppedState;

    if (!_input.isNull() && _buf && expectedCondition) {
        _buf->readAll();
    }
}

void AudioInput::readAudio()
{
    if (!_input->bytesReady()) {
        return;
    }

    QByteArray audio(_input->bytesReady(), 0);
    _buf->read(audio.data(), audio.size());
    _converter->appendAudio(audio);
}
