#ifndef AUDIOMIXER_H
#define AUDIOMIXER_H

#include <QMap>
#include <QTimer>
#include <QVector>
#include <QObject>
#include <cstring>

class AudioMixer : public QObject
{
    Q_OBJECT
public:
    AudioMixer(int outDurationMs = 10, QObject *parent = nullptr);

    int getSampleRate() const;
    /// In bytes
    int getSampleSize() const;

    /// In ms
    uint16_t getOutputAudioDuration() const;
    void setOutputAudioDuration(uint16_t ms);

    /**
     *  Emit null bytearrays with GetOutputAudioDuration() size
     *  to prevent audio stream from stopping. True by default
     */
    void setSilenceGeneration(bool state);

    void clear();

signals:
    /// 1. Set bytearray in ms on SetAudioDuration
    /// 2. Starting emit after first AppendAudio() call
    void audioReady(const QByteArray& audio);

public slots:
    /// PCMs for mixering audio flows by flow id
    void appendAudio(uint32_t id, const QByteArray& audio);

private slots:
    /// Формирование фрейма
    void prepareMixedAudio();

private:

    QTimer _timer;
    bool _silenceGen;
    uint16_t _timeInterval;
    // mixed samples
    QVector<int16_t> _stash;
    // pair: id audio flow and location for the future record
    QMap<uint16_t, uint16_t> _flows;

    void updateTimer();
    void initConnect();
    void writeAudio(uint32_t id, const QVector<int16_t> &audio);
    static inline int saturate(int left, int right);
    static inline QVector<int16_t> byteArrayToSamples(const QByteArray audio);
};

#endif // AUDIOMIXER_H
