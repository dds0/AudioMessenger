#include "NetCommunication.h"
#include "Common/SettingsStash.h"

NetCommunication::NetCommunication(QObject *parent)
    : QObject{parent},
      _interfaces(QNetworkInterface::allInterfaces())
{
    initConnections();
}

void NetCommunication::appendAddress(
    QHostAddress addr, QNetworkInterface interface)
{
    auto addrStr = addr.toString();
    auto ifaceStr = interface.name();

    auto command = QString("ip a add %1/24 dev %2").arg(addrStr).arg(ifaceStr);
    qDebug() << "Use command: " << command;

    std::system(command.toStdString().c_str());
}

void NetCommunication::removeAddress(QHostAddress src, QHostAddress multicast)
{
    auto addrStr = src.toString();
    auto multicastStr = multicast.toString();

    auto command = QString("ip r del %1 via %2").arg(addrStr).arg(multicastStr);
    qDebug() << "Use command: " << command;

    std::system(command.toStdString().c_str());
}

void NetCommunication::removeAddress(QHostAddress addr, QNetworkInterface interface)
{
    auto addrStr = addr.toString();
    auto ifaceStr = interface.name();

    auto command = QString("ip a del %1/24 dev %2").arg(addrStr).arg(ifaceStr);
    qDebug() << "Use command: " << command;

    std::system(command.toStdString().c_str());
}

QList<QNetworkInterface> NetCommunication::getInterfaces() const
{
    return _interfaces;
}

QNetworkInterface NetCommunication::getInterface(int ifaceIndex) const
{
    for (auto&& interface: _interfaces)
    {
        if (interface.index() == ifaceIndex) {
            return interface;
        }
    }
    return {};
}

void NetCommunication::appendAddress(QHostAddress src, QHostAddress multicast)
{
    auto addrStr = src.toString();
    auto multicastStr = multicast.toString();

    auto command = QString("ip r add %1 via %2").arg(addrStr).arg(multicastStr);
    qDebug() << "Use command: " << command;

    std::system(command.toStdString().c_str());
}

void NetCommunication::updateInterfaces()
{
    _interfaces = QNetworkInterface::allInterfaces();
    emit interfacesUpdated(_interfaces);
}

void NetCommunication::initConnections()
{
    connect(&_interfaceUpdateTimer, &QTimer::timeout,
        this, &NetCommunication::updateInterfaces);

    _interfaceUpdateTimer.start(5000);
}

