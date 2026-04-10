#ifndef CONNECTIONLOGIC_H
#define CONNECTIONLOGIC_H

#include <QTimer>
#include <QObject>
#include "Utils/ConnectionState.h"

class AMCDP_Client;

class ConnectionLogic : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionLogic(AMCDP_Client* client, QObject *parent = nullptr);

    void tryStartCall();
    void tryApplyCall();

    void tryEndCall();
    void tryDenyCall();

signals:
    void connectionStatusChange(ConnectionState);

private:
    ConnectionState _status;

    QTimer _disconnectTimer;
    QTimer _sendActivityStatus, _netProblemTimer;

    AMCDP_Client* _client;

    void initTimers();
    void initConnections();
};

#endif // CONNECTIONLOGIC_H
