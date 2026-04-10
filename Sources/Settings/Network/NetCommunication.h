#ifndef NETCOMMUNICATION_H
#define NETCOMMUNICATION_H

#include <QTimer>
#include <QObject>
#include <QHostAddress>
#include <QNetworkInterface>

class NetCommunication : public QObject
{
    Q_OBJECT
public:
    explicit NetCommunication(QObject *parent = nullptr);

    void updateInterfaces();

    QList<QNetworkInterface> getInterfaces() const;
    QNetworkInterface getInterface(int ifaceIndex) const;

    static void appendAddress(QHostAddress src, QHostAddress multicast);
    static void appendAddress(QHostAddress addr, QNetworkInterface interface);

    static void removeAddress(QHostAddress src, QHostAddress multicast);
    static void removeAddress(QHostAddress addr, QNetworkInterface interface);

signals:
    void interfacesUpdated(const QList<QNetworkInterface>& interfaces);

private:
    QTimer _interfaceUpdateTimer;
    QList<QNetworkInterface> _interfaces;

    void initConnections();
};

#endif // NETCOMMUNICATION_H
