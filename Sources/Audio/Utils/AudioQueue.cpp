#include "AudioQueue.h"

AudioQueue::AudioQueue(bool isActive, QObject *parent) : QObject(parent)
{
    setActive(isActive);

    _timer.setTimerType(Qt::PreciseTimer);
    connect(&_timer, &QTimer::timeout, this, &AudioQueue::dequeue);
}

void AudioQueue::enqueue(const AC::AudioFrame &frame)
{
    _queue.push(frame);
    activate();
}

void AudioQueue::enqueue(const std::vector<AC::AudioFrame> &frames)
{
    for(const auto& frame : frames)
        _queue.push(frame);

    activate();
}

void AudioQueue::clear()
{
    deactivate();
    _queue = {};

    emit cleared();
}

void AudioQueue::setActive(bool isActive)
{
    _isActive = isActive;

    if(_isActive) {
        activate();
    } else {
        deactivate();
    }
}

bool AudioQueue::isActive() const
{
    return _isActive;
}

bool AudioQueue::isEmpty() const
{
    return _queue.empty();
}

void AudioQueue::activate()
{
    if(! _timer.isActive() && _isActive) {
        _timer.start();
    }
}

void AudioQueue::deactivate()
{
    _timer.stop();
    _timer.setInterval(0);
}

void AudioQueue::checkInterval(int ms)
{
    if(ms != _timer.interval()) {
        _timer.setInterval(_queue.front().duration);
    }
}

void AudioQueue::dequeue()
{
    if(! _queue.empty())
    {
        checkInterval(_queue.front().duration);

        emit dequeued(_queue.front());
        _queue.pop();
    }
    else {
        deactivate();
        emit emptied();
    }
}
