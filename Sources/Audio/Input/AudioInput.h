#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QTimer>
#include <QObject>
#include <QIODevice>
#include <QAudioInput>
#include <QSharedPointer>

class PacketConverter;

class AudioInput : public QObject
{
    Q_OBJECT
public:
    explicit AudioInput(QObject *parent = nullptr);
    ~AudioInput();

    void setFrameSize(uint8_t ms);
    void setInputDevice(const QAudioDeviceInfo& device);

    void clear();

signals:
    void errorOccurred(QString error);
    void audioReady(const QByteArray& audio);

private slots:
    void readAudio();

private:
    QIODevice* _buf;

    QAudioDeviceInfo _device;
    PacketConverter* _converter;
    QSharedPointer<QAudioInput> _input;
};

#endif // AUDIOINPUT_H
