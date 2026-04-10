#ifndef CALLTAB_H
#define CALLTAB_H

#include <QWidget>
#include "Audio/Utils/ConnectionState.h"

class SettingsStash;

namespace Ui { class CallTab; }

class CallTab : public QWidget
{
    Q_OBJECT

public:
    explicit CallTab(SettingsStash* stash, QWidget *parent = nullptr);
    ~CallTab();

signals:
    void startCall();
    void applyCall();

    void denyCall();
    void endCall();

    void updateFrameSize(uint8_t ms);
    void updateVolumeLevel(float coefficient); // [0; 2.0]

public slots:
    void setNoiseLevel(uint8_t noise);
    void updateConnectionState(ConnectionState);

private:
    Ui::CallTab *ui;

    SettingsStash* _stash;

    void initUi();
    void initIcons();
    void initConnections();

    void setFrameSize();
    void lockCommunicationButtons();
    void logStatus(ConnectionState);
};

#endif // CALLTAB_H
