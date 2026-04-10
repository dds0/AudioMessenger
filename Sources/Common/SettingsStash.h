#ifndef SETTINGSSTASH_H
#define SETTINGSSTASH_H

#include <QObject>
#include <QSettings>

#include <QHostAddress>
#include <QAudioDeviceInfo>
#include <QNetworkInterface>

class SettingsStash : public QObject
{
    Q_OBJECT
public:
    explicit SettingsStash(QObject *parent = nullptr);

    void load(QString filePath = "AmSettings.ini");
    void save(QString filePath = "AmSettings.ini");

    uint8_t getFrameSize() const;
    uint8_t getVadModeIndex() const;

    uint8_t getDeviceId() const;
    uint16_t getSrcPort() const;
    uint16_t getDstPort() const;
    int getInterfaceIndex() const;

    QHostAddress getSrcAddress() const;
    QHostAddress getDstAddress() const;

    QString getInputDeviceName() const;
    QString getOutputDeviceName() const;

signals:
    void loaded();
    void saved();

public slots:
    void setFrameSize(uint8_t ms);
    void setVadModeIndex(uint8_t index);

    void setSrcPort(uint16_t port);
    void setDstPort(uint16_t port);
    void setDeviceId(uint8_t deviceId);
    void setInterfaceIndex(int interfaceIndex);

    void setSrcAddress(QHostAddress addr);
    void setDstAddress(QHostAddress addr);

    void setInputDevice(QAudioDeviceInfo device);
    void setOutputDevice(QAudioDeviceInfo device);

private:
    QSharedPointer<QSettings> _settings;
};

#endif // SETTINGSSTASH_H
