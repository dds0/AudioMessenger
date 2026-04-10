#include "AudioMixer.h"
#include "Audio/Utils/AudioConstants.h"

// 3 ms
const int leftSpacing = AC::BytesPerMillisecond / AC::BytesInSample * 3;

AudioMixer::AudioMixer(int outDurationMs, QObject *parent)
    : QObject(parent), _silenceGen(true), _timeInterval(outDurationMs)
{
    _stash.reserve(AC::BytesInSample * outDurationMs * 2);
    _timer.setTimerType(Qt::PreciseTimer);

    initConnect();
}

int AudioMixer::getSampleRate() const
{
    return AC::TargetSampleRate;
}

void AudioMixer::appendAudio(uint32_t id, const QByteArray &audio)
{
    // if flow not exist, adding
    if (!_flows.contains(id)) {
        _flows.insert(id, leftSpacing);
    }

    writeAudio(id, byteArrayToSamples(audio));

    if (!_timer.isActive()) {
        _timer.start(_timeInterval);
    }
}

void AudioMixer::setOutputAudioDuration(uint16_t ms)
{
    _timeInterval = ms;
}

uint16_t AudioMixer::getOutputAudioDuration() const
{
    return _timeInterval;
}

void AudioMixer::setSilenceGeneration(bool state)
{
    _silenceGen = state;
}

int AudioMixer::getSampleSize() const
{
    return AC::BytesPerMillisecond;
}

void AudioMixer::prepareMixedAudio()
{
    int samplesForSend = _timeInterval * AC::SamplesInMs;

    if (_stash.isEmpty() && !_silenceGen) {
        _timer.stop(); return;
    }

    // if we have enought samples
    if (samplesForSend > _stash.size()) {
        _stash.append(QVector<int16_t>(samplesForSend - _stash.size(), 0));
    }

    // update location for the next record
    for (auto& flow: _flows) {
        int newPoint = flow - samplesForSend;
        flow = newPoint > leftSpacing ? newPoint : leftSpacing;
    }

    // send & delete
    auto bytes = samplesForSend * AC::BytesInSample;
    emit audioReady(QByteArray((char*)_stash.data(), bytes));
    _stash.remove(0, samplesForSend);

    updateTimer();
}

void AudioMixer::updateTimer()
{
    if (_timeInterval != _timer.interval()) {
        _timer.start(_timeInterval);
    }
}

void AudioMixer::initConnect()
{
    connect(&_timer, &QTimer::timeout, this, &AudioMixer::prepareMixedAudio);
}

void AudioMixer::writeAudio(uint32_t id, const QVector<int16_t>& audio)
{
    auto targetFlow = _flows.find(id);

    // if we don't have empty space
    int endWritePoint = targetFlow.value() + audio.size();
    if (endWritePoint > _stash.size()) {
        _stash.append(QVector<int16_t>(endWritePoint - _stash.size() , 0));
    }

    // record
    int writePoint = targetFlow.value();
    for (int i = 0; i < audio.size(); ++i)
    {
        _stash[i + writePoint] = saturate(_stash[i + writePoint], audio[i]);
    }

    // update location for next record
    targetFlow.value() = endWritePoint;
}

int AudioMixer::saturate(int left, int right)
{
    if (left + right >= INT16_MAX) {
        return INT16_MAX;
    } else if(left + right <= INT16_MIN) {
        return INT16_MIN;
    }

    return left + right;
}

QVector<int16_t> AudioMixer::byteArrayToSamples(const QByteArray audio)
{
    QVector<int16_t> samples(audio.size() / AC::BytesInSample, 0);
    std::memcpy(samples.data(), audio.data(), audio.size());

    return samples;
}











