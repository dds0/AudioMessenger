#ifndef PACKETCONVERTER_H
#define PACKETCONVERTER_H

#include <QObject>
#include <chrono>

/// Deviding audio flow into arrays with the same size
class PacketConverter : public QObject
{
    Q_OBJECT
public:
    explicit PacketConverter(int outputAudioDuration = 10, QObject *parent = nullptr);

    /// set raw pcm specified duration frame
    void setOutputAudioDuration(uint16_t ms);

    /// return audio duration in ms
    int getOutputAudioDuration() const;

    /// add raw pcm to internal buffer
    void appendAudio(const QByteArray& audio);

    /// return in bytes
    int getCurrentBufferSize() const;

    void clear();

signals:
    /// return raw pcm with duration,
    /// wich set on SetOutputAudioDuration(int ms);
    void audioReady(const QByteArray& audio);

private:
    int _frameDuration;
    QByteArray _buffer;

    void CheckBuffer();
};

#endif // PACKETCONVERTER_H
