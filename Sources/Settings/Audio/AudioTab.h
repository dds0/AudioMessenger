#ifndef AUDIOTAB_H
#define AUDIOTAB_H

#include <QWidget>
#include <QHostAddress>
#include <QAudioDeviceInfo>
#include "Audio/Utils/AudioConstants.h"
#include "Audio/Utils/ConnectionState.h"

class SettingsStash;
class AudioDeviceManager;

namespace Ui { class AudioTab; }

class AudioTab : public QWidget
{
    Q_OBJECT

public:
    explicit AudioTab(SettingsStash* stash, QWidget *parent = nullptr);
    ~AudioTab();

signals:
    void updateVadMode(AC::VadMode mode);
    void updateSelfListeningTestState(bool state);

    void updateInputDevice(const QAudioDeviceInfo& device);
    void updateOutputDevice(const QAudioDeviceInfo& device);

public slots:
    void setNoiseLevel(uint8_t noise);
    void updateConnectionState(ConnectionState);

private:
    Ui::AudioTab *ui;

    SettingsStash* _stash;
    AudioDeviceManager* _deviceManager;

    void initUi();

    void initFilterModes();
    void initConnections();
    void initAudioDeviceManagerConnections();

    void setVadMode();
    void setInputDevice(QString deviceName);
    void setOutputDevice(QString deviceName);
};

#endif // AUDIOTAB_H
