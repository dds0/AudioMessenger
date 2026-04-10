#ifndef AUDIOQUEUE_H
#define AUDIOQUEUE_H

#include <queue>
#include <QTimer>
#include <QObject>

#include "AudioConstants.h"

class AudioQueue : public QObject
{
    Q_OBJECT
public:
    explicit AudioQueue(bool isActive = true, QObject *parent = nullptr);

    void enqueue(const AC::AudioFrame& frame);
    void enqueue(const std::vector<AC::AudioFrame>& frames);

    void clear();
    bool isEmpty() const;

    bool isActive() const;
    void setActive(bool isActive);

signals:
    void emptied();
    void cleared();

    void dequeued(const AC::AudioFrame& frame);

private:
    bool _isActive = true;

    QTimer _timer;
    std::queue<AC::AudioFrame> _queue;

    inline void activate();
    inline void deactivate();
    inline void checkInterval(int ms);

private slots:
    void dequeue();
};

#endif // AUDIOQUEUE_H
