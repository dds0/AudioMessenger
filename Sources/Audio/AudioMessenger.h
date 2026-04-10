#ifndef AUDIOMESSENGER_H
#define AUDIOMESSENGER_H

#include <QObject>
#include <QHostAddress>
#include <QAudioDeviceInfo>

#include "Utils/AudioConstants.h"
#include "Utils/ConnectionState.h"

class AMCDP_Client;
class SoundEffects;
class ConnectionLogic;

class AudioOutput;
class AudioMixer;

class AudioInput;
class AudioQueue;
class AudioSwitch;
class VoiceAudioDetector;
class SelfListeningSwitch;

class AudioMessenger : public QObject
{
    Q_OBJECT
public:
    explicit AudioMessenger(QObject *parent = nullptr);

    void setFrameSize(uint8_t ms);
    void setVadMode(AC::VadMode mode);

    void setVolumeLevel(float coefficient);
    void setSelfListeningTestState(bool state);

    void setDeviceId(uint8_t deviceId);
    void setInterfaceIndex(uint index);
    void setSource(QHostAddress addr, uint16_t port);
    void setDestination(QHostAddress addr, uint16_t port);

    void setInputDevice(const QAudioDeviceInfo& device);
    void setOutputDevice(const QAudioDeviceInfo& device);

signals:
    void connectionStatusChange(ConnectionState);

    void currentNoiseLevel(uint8_t noise); // [0: 100]

public slots:
    void startCall();
    void applyCall();

    void endCall();
    void denyCall();

private:
    AudioInput* _input;
    AudioOutput* _output;

    AudioMixer* _outMixer;
    AudioQueue* _inQueue;
    AudioSwitch* _inOutGateway;
    VoiceAudioDetector* _inVad;
    SelfListeningSwitch* _inSelfListening;

    AMCDP_Client* _client;
    SoundEffects* _effects;
    ConnectionLogic* _connectionLogic;

    void initConnections();
    uint32_t getAudioFlowId(QHostAddress addr) const;

    void initInputConnections();
    void initOutputConnections();
    void initSelfListeningConnections();
};

#endif // AUDIOMESSENGER_H
