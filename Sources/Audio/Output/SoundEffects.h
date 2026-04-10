#ifndef SOUNDEFFECTS_H
#define SOUNDEFFECTS_H

#include <QMap>
#include <QObject>
#include <QByteArray>
#include <QSoundEffect>
#include "Audio/ConnectionLogic.h"
#include "Audio/Utils/AudioConstants.h"

class SoundEffects : public QObject
{
    Q_OBJECT
public:
    explicit SoundEffects(QObject *parent = nullptr);

public slots:
    void updateConnectionState(ConnectionState);

private:
    QSoundEffect* _deninedCallEffect;
    QSoundEffect* _incomingCallEffect;
    QSoundEffect* _outgoingCallEffect;
};

#endif // SOUNDEFFECTS_H
