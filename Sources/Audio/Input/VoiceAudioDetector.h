#ifndef VOICEAUDIODETECTOR_H
#define VOICEAUDIODETECTOR_H

#include <QObject>
#include "Audio/Utils/AudioConstants.h"
#include "WebRtcAudio/vad/include/webrtc_vad.h"

class VoiceAudioDetector : public QObject
{
    Q_OBJECT
public:
    explicit VoiceAudioDetector(QObject *parent = nullptr);

    bool setMode(AC::VadMode mode);
    AC::VadMode getMode() const;

    ~VoiceAudioDetector();

signals:
    void audioReady(const QByteArray& pcm);

public slots:
    void appendAudio(const QByteArray& pcm);

private:
    AC::VadMode _mode;
    VadInst* _vad = NULL;
};

#endif // VOICEAUDIODETECTOR_H
