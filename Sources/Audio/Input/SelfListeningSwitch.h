#ifndef SELFLISTENINGSWITCH_H
#define SELFLISTENINGSWITCH_H

#include <QObject>

class SelfListeningSwitch : public QObject
{
    Q_OBJECT
public:
    explicit SelfListeningSwitch(QObject *parent = nullptr);

    /// Set id for audio flow
    void setFlowId(uint32_t id);

signals:
    void audioReady(uint32_t id, const QByteArray& audio);

public slots:
    /// On/off permission for emit audioReady
    void setGateway(bool state);

    void appendAudio(const QByteArray& audio);

private:
    uint32_t _id;
    bool _gatewayState;
};


#endif // SELFLISTENINGSWITCH_H
