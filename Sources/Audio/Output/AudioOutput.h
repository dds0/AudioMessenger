#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include <QTimer>
#include <QObject>
#include <QAudioOutput>
#include <QSharedPointer>

class AudioOutput : public QObject
{
    Q_OBJECT
public:
    explicit AudioOutput(QObject *parent = nullptr);
    ~AudioOutput();

    void setOutputDevice(const QAudioDeviceInfo& device);

    /// [0; 2.0]
    float getVolume() const;
    void setVolume(float coefficient);

signals:
    void noiseLevel(int16_t highestSample);

public slots:
    void appendAudio(const QByteArray& audio);

private:
    QIODevice* _buf;

    int16_t _maxNoiseLevel{};
    QTimer _noiseLevelSenderTimer;

    float _volume;
    QAudioDeviceInfo _device;
    QSharedPointer<QAudioOutput> _output;

    QByteArray updateVolume(const QByteArray& audio);
};

#endif // AUDIOOUTPUT_H
