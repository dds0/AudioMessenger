#include "AudioConstants.h"

using namespace AC;

QAudioFormat AC::AudioFormat()
{
    QAudioFormat format;
    format.setSampleSize(16);
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setCodec("audio/pcm");
    format.setSampleType(QAudioFormat::SampleType::SignedInt);

    return format;
}
