#ifndef AMCDP_H
#define AMCDP_H

#include <QObject>
#include <optional>
#include "AMCDPSpace.h"

/// Audio messenger control & data protocol

class AMCDP : public QObject
{
    Q_OBJECT
public:
    explicit AMCDP(QObject *parent = nullptr);

    void setDeviceId(uint8_t deviceId);

signals:
    void receive_reqStartCall();
    void receive_replyStartCall();
    void receive_statusActivity();
    void receive_reqNoReplyEndCall();
    void receive_reqNoReplyDenyCall();
    void receive_reqNoReplyVoice(const AMCDPSPace::Voice& voice);

    void send(const QByteArray& bytes);

public slots:
    void send_reqStartCall();
    void send_replyStartCall();
    void send_statusActivity();
    void send_reqNoReplyEndCall();
    void send_reqNoReplyDenyCall();
    void send_reqNoReplyVoice(const AMCDPSPace::Voice& voice);

    void parse(QByteArray bytes);

private:
    uint8_t _deviceId;

    AMCDPSPace::Header deserializeHeader(
        QByteArray& bytes, bool& state);
    QByteArray serializeHeader(AMCDPSPace::TypeCommand type,
        AMCDPSPace::CodeCommand command);

    std::optional<AMCDPSPace::Voice> parseVoice(QByteArray& bytes);
};

#endif // AMCDP_H
