#ifndef AMCDP_CLIENT_H
#define AMCDP_CLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QSharedPointer>
#include <QNetworkDatagram>

#include "Audio/Utils/AudioConstants.h"

class AMCDP;

/// Audio messenger control & data protocol client
class AMCDP_Client : public QObject
{
    Q_OBJECT
public:
    explicit AMCDP_Client(QObject *parent = nullptr);

    QHostAddress getDestinationAddr() const;

    void setDeviceId(uint8_t deviceId);
    void setInterfaceIndex(uint interfaceIndex);
    void setSource(QHostAddress addr, uint16_t port);
    void setDestination(QHostAddress addr, uint16_t port);

    void sendStatusActivity();

    void sendRequestStartCall();

    void sendReplyStartCall();

    void sendReqNoReplyEndCall();
    void sendReqNoReplyDenyCall();
    void sendReqNoRepyVoice(const AC::AudioFrame& frame);

signals:
    void activityStatusReceived();

    void requestStartCallReceived();

    void replyStartCallReceived();

    void reqNoReplyEndCallReceived();
    void reqNoReplyDenyCallReceived();
    void reqNoRepyVoiceReceived(const AC::AudioFrame& frame);

private:
    AMCDP* _proto;

    uint _interfaceIndex{};
    uint16_t _srcPort, _dstPort;
    QHostAddress _srcAddr, _dstAddr;

    QSharedPointer<QUdpSocket> _socket;

    void initConnections();
    QSharedPointer<QUdpSocket> getSocket(QHostAddress addr, uint16_t port);

    void parseDatagram();
    void sendDatagram(const QByteArray& bytes);
};

#endif // AMCDP_CLIENT_H
