#include "ConnectionLogic.h"
#include "Network/Proto/AMCDP_Client.h"

int sendStatusInterval = 500;
int netProblemInterval = 1500;
int disconnectInterval = 6000;

ConnectionLogic::ConnectionLogic(AMCDP_Client *client, QObject *parent)
    : QObject{parent},
      _status(Idle),
      _client(client)
{
    initTimers();
    initConnections();
}

void ConnectionLogic::tryStartCall()
{
    if (_status == Idle)
    {
        _client->sendRequestStartCall();

        _status = OutgoingCall;
        emit connectionStatusChange(_status);

        _disconnectTimer.start();
        _netProblemTimer.start();
        _sendActivityStatus.start();
    }
}

void ConnectionLogic::tryApplyCall()
{
    if (_status == IncomingCall)
    {
        _client->sendReplyStartCall();

        _status = Active;
        emit connectionStatusChange(_status);

        _disconnectTimer.start();
        _netProblemTimer.start();
        _sendActivityStatus.start();
    }
}

void ConnectionLogic::tryEndCall()
{
    if (_status == Active || _status == Missing || _status == OutgoingCall)
    {
        _client->sendReqNoReplyEndCall();

        _status = Ended;
        emit connectionStatusChange(_status);
        _status = Idle;
        emit connectionStatusChange(_status);

        _disconnectTimer.stop();
        _sendActivityStatus.stop();
    }
}

void ConnectionLogic::tryDenyCall()
{
    if (_status == IncomingCall)
    {
        _client->sendReqNoReplyDenyCall();

        _status = Denied;
        emit connectionStatusChange(_status);
        _status = Idle;
        emit connectionStatusChange(_status);
    }
}

void ConnectionLogic::initTimers()
{
    _disconnectTimer.setSingleShot(true);
    _netProblemTimer.setSingleShot(true);

    _disconnectTimer.setInterval(disconnectInterval);
    _netProblemTimer.setInterval(netProblemInterval);
    _sendActivityStatus.setInterval(sendStatusInterval);

    connect(&_sendActivityStatus, &QTimer::timeout,
        _client, &AMCDP_Client::sendStatusActivity);

    connect(&_netProblemTimer, &QTimer::timeout, this,
    [this]()
    {
        if (_status == Active)
        {
            _status = Missing;
            emit connectionStatusChange(_status);
        }
    });

    connect(&_disconnectTimer, &QTimer::timeout, this,
    [this]()
    {
        if (_status == OutgoingCall || _status == IncomingCall || _status == Missing)
        {
            _status = Denied;
            emit connectionStatusChange(_status);
            _status = Idle;
            emit connectionStatusChange(_status);

            _sendActivityStatus.stop();
        }
    });
}

void ConnectionLogic::initConnections()
{
    connect(_client, &AMCDP_Client::activityStatusReceived, this, [this]()
    {
        if (_status == Missing)
        {
            _status = Active;
            emit connectionStatusChange(_status);
        }

        if (_status == Active)
        {
            _disconnectTimer.start();
            _netProblemTimer.start();
        }
    });

    connect(_client, &AMCDP_Client::requestStartCallReceived, this, [this]()
    {
        if (_status == Idle)
        {
            _status = IncomingCall;
            emit connectionStatusChange(_status);

            _disconnectTimer.start();
        }
    });

    connect(_client, &AMCDP_Client::replyStartCallReceived, this, [this]()
    {
        if (_status == OutgoingCall)
        {
            _status = Active;
            emit connectionStatusChange(_status);
        }
    });

    connect(_client, &AMCDP_Client::reqNoReplyEndCallReceived, this, [this]()
    {
        if (_status == Active || _status == Missing)
        {
            _status = Ended;
            emit connectionStatusChange(_status);
            _status = Idle;
            emit connectionStatusChange(_status);

            _disconnectTimer.stop();
            _netProblemTimer.stop();
            _sendActivityStatus.stop();
        }
    });

    connect(_client, &AMCDP_Client::reqNoReplyDenyCallReceived, this, [this]()
    {
        if (_status == OutgoingCall || _status == IncomingCall || _status == Missing)
        {
            _status = Denied;
            emit connectionStatusChange(_status);
            _status = Idle;
            emit connectionStatusChange(_status);

            _disconnectTimer.stop();
            _netProblemTimer.stop();
            _sendActivityStatus.stop();
        }
    });
}
