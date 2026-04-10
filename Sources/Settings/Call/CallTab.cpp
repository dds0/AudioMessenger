#include "CallTab.h"
#include "ui_CallTab.h"

#include <QTime>
#include <QIcon>
#include <QStyle>
#include <QMessageBox>
#include "Common/SettingsStash.h"

CallTab::CallTab(SettingsStash *stash, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CallTab),
    _stash(stash)
{
    ui->setupUi(this);
    initConnections();
}

CallTab::~CallTab()
{
    delete ui;
}

void CallTab::setNoiseLevel(uint8_t noise)
{
    ui->hS_outputNoise->setValue(noise);
}

void CallTab::updateConnectionState(ConnectionState status)
{
    lockCommunicationButtons();

    switch (status)
    {
        case ConnectionState::Active:
        case ConnectionState::Missing:
        case ConnectionState::OutgoingCall:
            ui->pB_endCall->setEnabled(true);
            break;

        case ConnectionState::Idle:
        case ConnectionState::Ended:
        case ConnectionState::Denied:
            ui->pB_startCall->setEnabled(true);
            break;

        case ConnectionState::IncomingCall:
            ui->pB_denyCall->setEnabled(true);
            ui->pB_applyCall->setEnabled(true);

        default:
            break;
    }

    logStatus(status);
    ui->rB_connectionState->setChecked(status == Active);
}

void CallTab::initUi()
{
    initIcons();
    updateConnectionState(ConnectionState::Idle);

    auto packetMsSize = _stash->getFrameSize();
    ui->l_frameSize->setText(QString::number(packetMsSize));

    emit updateFrameSize(packetMsSize);
}

void CallTab::initIcons()
{
    auto volumeUp = qApp->style()->standardIcon(QStyle::SP_MediaVolume);
    auto volumeDown = qApp->style()->standardIcon(QStyle::SP_MediaVolumeMuted);

    auto apply = qApp->style()->standardIcon(QStyle::SP_DialogApplyButton);
    auto cancel = qApp->style()->standardIcon(QStyle::SP_DialogCancelButton);

    ui->pB_volumeUp->setIcon(volumeUp);
    ui->pB_volumeDown->setIcon(volumeDown);

    ui->pB_endCall->setIcon(cancel);
    ui->pB_denyCall->setIcon(cancel);
    ui->pB_startCall->setIcon(apply);
    ui->pB_applyCall->setIcon(apply);
}

void CallTab::initConnections()
{
    connect(ui->pB_setFrameSize, &QPushButton::clicked,
        this, &CallTab::setFrameSize);

    connect(ui->pB_endCall, &QPushButton::clicked,
        this, &CallTab::endCall);
    connect(ui->pB_denyCall, &QPushButton::clicked,
        this, &CallTab::denyCall);
    connect(ui->pB_startCall, &QPushButton::clicked,
        this, &CallTab::startCall);
    connect(ui->pB_applyCall, &QPushButton::clicked,
        this, &CallTab::applyCall);

    connect(ui->pB_volumeUp, &QPushButton::clicked, this,
    [this](){ ui->vS_volume->setValue(ui->vS_volume->value() + 1); });
    connect(ui->pB_volumeDown, &QPushButton::clicked, this,
    [this](){ ui->vS_volume->setValue(ui->vS_volume->value() - 1); });

    connect(ui->vS_volume, &QSlider::valueChanged, this,
    [this](int value) { emit updateVolumeLevel(value / 10.); });

    connect(_stash, &SettingsStash::loaded, this, &CallTab::initUi);
}

void CallTab::lockCommunicationButtons()
{
    ui->pB_endCall->setEnabled(false);
    ui->pB_denyCall->setEnabled(false);
    ui->pB_startCall->setEnabled(false);
    ui->pB_applyCall->setEnabled(false);
}

void CallTab::logStatus(ConnectionState status)
{
    QString message;

    switch (status)
    {
        case ConnectionState::Active:
            message = tr("Correct connection");
            break;
        case ConnectionState::Missing:
            message = tr("Problem with net connection");
            break;
        case ConnectionState::OutgoingCall:
            message = tr("Calling...");
            break;
        case ConnectionState::Idle:
            message = tr("Idle state");
            break;
        case ConnectionState::Ended:
            message = tr("Call ended sucessfully");
            break;
        case ConnectionState::Denied:
            message = tr("Сall was cancelled / disconnected");
            break;
        case ConnectionState::IncomingCall:
            message = tr("Incoming call...");
            break;
        default:
            message = tr("Undefined state");
            break;
    }

    QString logLine("[%1]: %2");
    auto time = QTime::currentTime().toString("hh:mm:ss");

    ui->lW_Logger->addItem(logLine.arg(time).arg(message));
}

void CallTab::setFrameSize()
{
    auto strMs = ui->cB_frameSize->currentText();
    ui->l_frameSize->setText(strMs);

    emit updateFrameSize(strMs.toInt());
}
