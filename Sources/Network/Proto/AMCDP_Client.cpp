#include "AMCDP_Client.h"
#include "AMCDP.h"

AMCDP_Client::AMCDP_Client(QObject *parent)
    : QObject(parent),
      _proto(new AMCDP(this))
{
    initConnections();
}

QHostAddress AMCDP_Client::getDestinationAddr() const
{
    return _dstAddr;
}

void AMCDP_Client::setDeviceId(uint8_t deviceId)
{
    _proto->setDeviceId(deviceId);
}

void AMCDP_Client::setInterfaceIndex(uint interfaceIndex)
{
    _interfaceIndex = interfaceIndex;
}

void AMCDP_Client::setSource(QHostAddress addr, uint16_t port)
{
    _srcAddr = addr; _srcPort = port;
}

void AMCDP_Client::setDestination(QHostAddress addr, uint16_t port)
{
    _dstAddr = addr; _dstPort = port;
    _socket = getSocket(_dstAddr, _dstPort);
}

void AMCDP_Client::sendStatusActivity()
{
    _proto->send_statusActivity();
}

void AMCDP_Client::sendRequestStartCall()
{
    _proto->send_reqStartCall();
}

void AMCDP_Client::sendReplyStartCall()
{
    _proto->send_replyStartCall();
}

void AMCDP_Client::sendReqNoReplyEndCall()
{
    _proto->send_reqNoReplyEndCall();
}

void AMCDP_Client::sendReqNoReplyDenyCall()
{
    _proto->send_reqNoReplyDenyCall();
}

void AMCDP_Client::sendReqNoRepyVoice(const AC::AudioFrame &frame)
{
    _proto->send_reqNoReplyVoice(frame);
}

void AMCDP_Client::initConnections()
{
    connect(_proto, &AMCDP::send,
        this, &AMCDP_Client::sendDatagram);

    connect(_proto, &AMCDP::receive_reqStartCall,
        this, &AMCDP_Client::requestStartCallReceived);
    connect(_proto, &AMCDP::receive_replyStartCall,
        this, &AMCDP_Client::replyStartCallReceived);

    connect(_proto, &AMCDP::receive_statusActivity,
        this, &AMCDP_Client::activityStatusReceived);

    connect(_proto, &AMCDP::receive_reqNoReplyEndCall,
        this, &AMCDP_Client::reqNoReplyEndCallReceived);
    connect(_proto, &AMCDP::receive_reqNoReplyDenyCall,
        this, &AMCDP_Client::reqNoReplyDenyCallReceived);

    connect(_proto, &AMCDP::receive_reqNoReplyVoice,
        this, &AMCDP_Client::reqNoRepyVoiceReceived);
}

QSharedPointer<QUdpSocket> AMCDP_Client::getSocket(QHostAddress addr, uint16_t port)
{
    auto modes = QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint;

    QSharedPointer<QUdpSocket> sock (new QUdpSocket);
    sock->bind(QHostAddress::AnyIPv4, port, modes);

    if(addr.isMulticast())
    {
        sock->joinMulticastGroup(addr);
        sock->setSocketOption(QAbstractSocket::MulticastLoopbackOption, true);
    }

    connect(sock.get(), &QUdpSocket::readyRead,
        this, &AMCDP_Client::parseDatagram);

    return sock;
}

void AMCDP_Client::parseDatagram()
{
    auto datagram = _socket->receiveDatagram();

    if (_interfaceIndex == datagram.interfaceIndex()) {
        _proto->parse(datagram.data());
    }
}

void AMCDP_Client::sendDatagram(const QByteArray &bytes)
{
    QNetworkDatagram datagram;

    if (_interfaceIndex)
    {
        datagram.setSender(_srcAddr, _srcPort);
        datagram.setInterfaceIndex(_interfaceIndex);
    }
    datagram.setData(bytes);
    datagram.setDestination(_dstAddr, _dstPort);

    if (!_socket.isNull()) {
        _socket->writeDatagram(datagram);
    }
}
