#include "SoundEffects.h"
#include <QThread>
#include <QDebug>
#include <QFileInfo>

SoundEffects::SoundEffects(QObject *parent)
    : QObject(parent),
      _deninedCallEffect(new QSoundEffect(this)),
      _incomingCallEffect(new QSoundEffect(this)),
      _outgoingCallEffect(new QSoundEffect(this))
{
    auto initEffect = [](auto&& effect, auto path)
    {
        effect->setSource(QUrl::fromLocalFile(path));
        effect->setVolume(0.25f);
    };
    initEffect(_deninedCallEffect, "://sounds/deniedCall.wav");
    initEffect(_incomingCallEffect, "://sounds/incomingCall.wav");
    initEffect(_outgoingCallEffect, "://sounds/outgoingCall.wav");

    _incomingCallEffect->setLoopCount(QSoundEffect::Infinite);
    _outgoingCallEffect->setLoopCount(QSoundEffect::Infinite);
}

void SoundEffects::updateConnectionState(ConnectionState state)
{
    if (state == Denied)
    {
        _deninedCallEffect->setLoopCount(1);
        _deninedCallEffect->play();
    }

    if (state == OutgoingCall) {
        _outgoingCallEffect->play();
    }

    if (state == IncomingCall) {
        _outgoingCallEffect->play();
    }

    if (state == Active || state == Denied || state == Ended) {
        _incomingCallEffect->stop();
        _outgoingCallEffect->stop();
    }
}
