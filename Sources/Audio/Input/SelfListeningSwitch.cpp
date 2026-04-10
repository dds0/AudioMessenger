#include "SelfListeningSwitch.h"

SelfListeningSwitch::SelfListeningSwitch(QObject *parent)
    : QObject(parent), _gatewayState(false)
{}

void SelfListeningSwitch::setFlowId(uint32_t id)
{
    _id = id;
}

void SelfListeningSwitch::setGateway(bool state)
{
    _gatewayState = state;
}

void SelfListeningSwitch::appendAudio(const QByteArray &audio)
{
    if (_gatewayState) {
        emit audioReady(_id, audio);
    }
}
