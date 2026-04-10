#ifndef AUDIODEVICEMANAGER_H
#define AUDIODEVICEMANAGER_H

#include <QMap>
#include <QObject>
#include <QAudioDeviceInfo>

class AudioDeviceManager : public QObject
{
    Q_OBJECT
public:
    explicit AudioDeviceManager(QObject *parent = nullptr);

    bool isInProgress() const;

    QAudioDeviceInfo getInputDevice(QString deviceName) const;
    QAudioDeviceInfo getOutputDevice(QString deviceName) const;

    QList<QAudioDeviceInfo> getInputDevices() const;
    QList<QAudioDeviceInfo> getOutputDevices() const;

    void updateList();

signals:
    void listUpdated();

private:
    std::atomic<bool> _inProgress;
    QMap<QString, QAudioDeviceInfo> _input, _output;

    static void updateAudioDevices(std::atomic<bool>& progressState,
        QMap<QString, QAudioDeviceInfo> &inputDevs, QMap<QString, QAudioDeviceInfo> &outputDevs);
};

#endif // AUDIODEVICEMANAGER_H
