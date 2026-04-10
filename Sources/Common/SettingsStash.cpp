#include "SettingsStash.h"
#include "Audio/Utils/AudioConstants.h"

SettingsStash::SettingsStash(QObject *parent) : QObject(parent) {}

void SettingsStash::load(QString filePath)
{
    _settings.reset(new QSettings(filePath, QSettings::IniFormat));
    emit loaded();
}

void SettingsStash::save(QString filePath)
{
    QSettings saver(filePath, QSettings::IniFormat);

    for (auto&& key: _settings->allKeys()) {
        saver.setValue(key, _settings->value(key));
    }

    saver.sync();
    emit saved();
}

uint8_t SettingsStash::getFrameSize() const
{
    return _settings->value("frameSize", 20).toUInt();
}

uint8_t SettingsStash::getVadModeIndex() const
{
    return _settings->value("vadMode", AC::VadMode::Normal).toInt();
}

uint8_t SettingsStash::getDeviceId() const
{
    return _settings->value("deviceId").toUInt();
}

int SettingsStash::getInterfaceIndex() const
{
    return _settings->value("interface", 2).toUInt();
}

void SettingsStash::setSrcAddress(QHostAddress addr)
{
    _settings->setValue("srcAddr", addr.toString());
}

void SettingsStash::setDstAddress(QHostAddress addr)
{
    _settings->setValue("dstAddr", addr.toString());
}

uint16_t SettingsStash::getSrcPort() const
{
    return _settings->value("srcPort", 12345).toInt();
}

uint16_t SettingsStash::getDstPort() const
{
    return _settings->value("dstPort", 12345).toInt();
}

QHostAddress SettingsStash::getSrcAddress() const
{
    return QHostAddress(_settings->value("srcAddr", "127.0.0.1").toString());
}

QHostAddress SettingsStash::getDstAddress() const
{
    return QHostAddress(_settings->value("dstAddr", "224.0.0.21").toString());
}

QString SettingsStash::getInputDeviceName() const
{
    return _settings->value("input", "default").toString();
}

QString SettingsStash::getOutputDeviceName() const
{
    return _settings->value("output", "default").toString();
}

void SettingsStash::setFrameSize(uint8_t ms)
{
    _settings->setValue("frameSize", ms);
}

void SettingsStash::setVadModeIndex(uint8_t index)
{
    _settings->setValue("vadMode", index);
}

void SettingsStash::setSrcPort(uint16_t port)
{
    _settings->setValue("srcPort", port);
}

void SettingsStash::setDstPort(uint16_t port)
{
    _settings->setValue("dstPort", port);
}

void SettingsStash::setDeviceId(uint8_t deviceId)
{
    _settings->setValue("deviceId", deviceId);
}

void SettingsStash::setInterfaceIndex(int interfaceIndex)
{
    _settings->setValue("interface", interfaceIndex);
}

void SettingsStash::setInputDevice(QAudioDeviceInfo device)
{
    _settings->setValue("input", device.deviceName());
}

void SettingsStash::setOutputDevice(QAudioDeviceInfo device)
{
    _settings->setValue("output", device.deviceName());
}
