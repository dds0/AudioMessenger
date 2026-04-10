#include "AudioDeviceManager.h"

#include <QDebug>
#include <QThread>
#include "Audio/Utils/AudioConstants.h"

AudioDeviceManager::AudioDeviceManager(QObject *parent)
    : QObject{parent} {}

bool AudioDeviceManager::isInProgress() const
{
    return _inProgress;
}

QAudioDeviceInfo AudioDeviceManager::getInputDevice(QString deviceName) const
{
    if (_inProgress) {
        return {};
    }
    return _input.value(deviceName);
}

QAudioDeviceInfo AudioDeviceManager::getOutputDevice(QString deviceName) const
{
    if (_inProgress) {
        return {};
    }
    return _output.value(deviceName);
}

QList<QAudioDeviceInfo> AudioDeviceManager::getInputDevices() const
{
    if (_inProgress) {
        return {};
    }
    return _input.values();
}

QList<QAudioDeviceInfo> AudioDeviceManager::getOutputDevices() const
{
    if (_inProgress) {
        return {};
    }
    return _output.values();
}

void AudioDeviceManager::updateList()
{
    auto thread = QThread::create(
        std::bind(updateAudioDevices, std::ref(_inProgress), std::ref(_input), std::ref(_output))
    );

    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(thread, &QThread::finished, this, &AudioDeviceManager::listUpdated);

    _inProgress = true;
    thread->start();
}

void AudioDeviceManager::updateAudioDevices(std::atomic<bool> &progressState,
    QMap<QString, QAudioDeviceInfo> &inputDevs, QMap<QString, QAudioDeviceInfo> &outputDevs)
{
    inputDevs.clear();
    auto inputList = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);

    outputDevs.clear();
    auto outputList = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);

    auto format = AC::AudioFormat();
    auto infoMsg = QString("Found %1 device: %2");

    auto print = [infoMsg](auto&& devices, auto comment)
    {
        for (auto&& dev: devices) {
            qInfo() << infoMsg.arg(comment).arg(dev.deviceName());
        }
    };

    auto insert = [format](auto&& dst, auto&& src)
    {
        for (auto& dev: src)
        {
            if (!dev.isNull() && dev.isFormatSupported(format))
            {
                dst.insert(dev.deviceName(), dev);
            }
        }
    };

    insert(inputDevs, inputList);
    insert(outputDevs, outputList);

    print(inputDevs, "INPUT");
    print(outputDevs, "OUTPUT");

    inputDevs.insert("default", QAudioDeviceInfo::defaultInputDevice());
    outputDevs.insert("default", QAudioDeviceInfo::defaultOutputDevice());

    progressState = false;
}
