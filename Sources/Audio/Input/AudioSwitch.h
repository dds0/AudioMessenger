#ifndef AUDIOSWITCH_H
#define AUDIOSWITCH_H

#include <QObject>
#include "Audio/Utils/AudioConstants.h"

class AudioSwitch : public QObject
{
    Q_OBJECT
public:
    explicit AudioSwitch(QObject *parent = nullptr);

signals:
    void incomingAudioReady(const QByteArray& audio);
    void outgoingAudioFrameReady(const AC::AudioFrame& frame);

public slots:
    /// On/off permission for emit audioFrameReady
    void setGateway(bool state);

    void appendOutgoingAudio(const QByteArray& audio);
    void appendIncomingAudioFrame(const AC::AudioFrame& frame);

private:
    bool _gatewayState;
};

#endif // AUDIOSWITCH_H
