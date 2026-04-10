#ifndef STREAMCONFIGURATOR_H
#define STREAMCONFIGURATOR_H

#include <memory>

#include <QObject>
#include <QSharedPointer>

#include "CsBase/Audio/Codec.h"
#include "InterfacePrivate/InterCom/qt/DSP.h"

using namespace DSPSpace;

/// Отправка аудио-пакетов по протоколу DSP

class StreamConfigurator : public QObject
{
    Q_OBJECT
public:
    explicit StreamConfigurator(QObject *parent = nullptr);

    void SetProto(QWeakPointer<DSPSpace::DSP> proto);

    /// По какому каналу высылаются аудио
    void SetMainChannel(uint8_t channel);
    uint8_t GetChannel() const;

public slots:
    /// Отправка фрейма(-ов)
    void SendFrame(const AudioFrame& frame);
    void SendFrames(const std::vector<AudioFrame>& frames);

private:
    uint8_t _channel;
    QWeakPointer<DSPSpace::DSP> _proto;

    static enum_Codec ConvertCodecType(CodecType type);
};

#endif // STREAMCONFIGURATOR_H
