#include "AudioMessenger.h"

#include "Audio/ConnectionLogic.h"
#include "Audio/Output/SoundEffects.h"
#include "Network/Proto/AMCDP_Client.h"

#include "Audio/Output/AudioMixer.h"
#include "Audio/Output/AudioOutput.h"

#include "Audio/Input/AudioInput.h"
#include "Audio/Utils/AudioQueue.h"
#include "Audio/Input/AudioSwitch.h"
#include "Audio/Input/VoiceAudioDetector.h"
#include "Audio/Input/SelfListeningSwitch.h"

AudioMessenger::AudioMessenger(QObject *parent)
    : QObject{parent},
      _input(new AudioInput(this)),
      _output(new AudioOutput(this)),
      _outMixer(new AudioMixer(20, this)),
      _inQueue(new AudioQueue(true, this)),
      _inOutGateway(new AudioSwitch(this)),
      _inVad(new VoiceAudioDetector(this)),
      _inSelfListening(new SelfListeningSwitch(this)),
      _client(new AMCDP_Client(this)),
      _effects(new SoundEffects(this)),
      _connectionLogic(new ConnectionLogic(_client, this))
{
    initConnections();
    _inSelfListening->setFlowId(AC::selfListeningAudioFlowId);
}

void AudioMessenger::setVolumeLevel(float coefficient)
{
    _output->setVolume(coefficient);
}

void AudioMessenger::setSelfListeningTestState(bool state)
{
    _inSelfListening->setGateway(state);
}

void AudioMessenger::setDeviceId(uint8_t deviceId)
{
    _client->setDeviceId(deviceId);
}

void AudioMessenger::setInterfaceIndex(uint index)
{
    _client->setInterfaceIndex(index);
}

void AudioMessenger::setFrameSize(uint8_t ms)
{
    _input->setFrameSize(ms);
}

void AudioMessenger::setVadMode(AC::VadMode mode)
{
    _inVad->setMode(mode);
}

void AudioMessenger::setSource(QHostAddress addr, uint16_t port)
{
    _client->setSource(addr, port);
}

void AudioMessenger::setDestination(QHostAddress addr, uint16_t port)
{
    _client->setDestination(addr, port);
}

void AudioMessenger::setInputDevice(const QAudioDeviceInfo &device)
{
    _input->setInputDevice(device);
}

void AudioMessenger::setOutputDevice(const QAudioDeviceInfo &device)
{
    _output->setOutputDevice(device);
}

void AudioMessenger::startCall()
{
    _connectionLogic->tryStartCall();
}

void AudioMessenger::applyCall()
{
    _connectionLogic->tryApplyCall();
}

void AudioMessenger::endCall()
{
    _connectionLogic->tryEndCall();
}

void AudioMessenger::denyCall()
{
    _connectionLogic->tryDenyCall();
}

void AudioMessenger::initConnections()
{
    connect(_connectionLogic, &ConnectionLogic::connectionStatusChange,
        this, &AudioMessenger::connectionStatusChange);
    connect(_connectionLogic, &ConnectionLogic::connectionStatusChange,
        _effects, &SoundEffects::updateConnectionState);
    connect(_connectionLogic, &ConnectionLogic::connectionStatusChange,
        this, [this](ConnectionState state) {
        _inOutGateway->setGateway(state == Active || state == Missing);
    });

    initInputConnections();
    initOutputConnections();
    initSelfListeningConnections();
}

uint32_t AudioMessenger::getAudioFlowId(QHostAddress addr) const
{
    return addr.toIPv4Address();
}

void AudioMessenger::initInputConnections()
{
    connect(_input, &AudioInput::audioReady,
        _inVad, &VoiceAudioDetector::appendAudio);

    connect(_inVad, &VoiceAudioDetector::audioReady,
        _inOutGateway, &AudioSwitch::appendOutgoingAudio);

    connect(_inOutGateway, &AudioSwitch::outgoingAudioFrameReady,
        _inQueue, QOverload<const AC::AudioFrame&>::of(&AudioQueue::enqueue));

    connect(_inQueue, &AudioQueue::dequeued,
        _client, &AMCDP_Client::sendReqNoRepyVoice);
}

void AudioMessenger::initOutputConnections()
{
    connect(_client, &AMCDP_Client::reqNoRepyVoiceReceived,
        _inOutGateway, &AudioSwitch::appendIncomingAudioFrame);

    connect(_inOutGateway, &AudioSwitch::incomingAudioReady, this,
        [this](auto&& audio)
    {
        auto audioFlowId = getAudioFlowId(_client->getDestinationAddr());
        _outMixer->appendAudio(audioFlowId, audio);
    });

    connect(_outMixer, &AudioMixer::audioReady,
        _output, &AudioOutput::appendAudio);

    connect(_output, &AudioOutput::noiseLevel, this,
        [=](int16_t sample) {
        emit currentNoiseLevel((float(sample) / INT16_MAX) * 100);
    });
}

void AudioMessenger::initSelfListeningConnections()
{
    connect(_inVad, &VoiceAudioDetector::audioReady,
        _inSelfListening, &SelfListeningSwitch::appendAudio);
    connect(_inSelfListening, &SelfListeningSwitch::audioReady,
        _outMixer, &AudioMixer::appendAudio);
}
