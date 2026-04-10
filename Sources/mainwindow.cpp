#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QLabel>
#include <QStatusBar>

#include "Common/SettingsStash.h"

#include "Settings/Call/CallTab.h"
#include "Settings/Audio/AudioTab.h"
#include "Settings/Network/NetTab.h"

#include "Audio/AudioMessenger.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      _stash(new SettingsStash(this)),
      _callSettings(new CallTab(_stash, this)),
      _networkSettings(new NetTab(_stash, this)),
      _audioSettings(new AudioTab(_stash, this)),
      _audioMessenger(new AudioMessenger(this))
{
    ui->setupUi(this);

    setWindowIcon(QIcon("://logo.png"));
    this->setWindowTitle("Audio Messenger");
    this->statusBar()->addWidget(new QLabel(QString(TAG), this));

    ui->tab_call->layout()->addWidget(_callSettings);
    ui->page_sound->layout()->addWidget(_audioSettings);
    ui->page_network->layout()->addWidget(_networkSettings);

    initNetConnections();
    initCallConnections();
    initAudioConnections();

    _stash->load();
}

MainWindow::~MainWindow()
{
    _stash->save();
    delete ui;
}

void MainWindow::initNetConnections()
{
    connect(_networkSettings, &NetTab::updateSrc,
        _audioMessenger, &AudioMessenger::setSource);
    connect(_networkSettings, &NetTab::updateDst,
        _audioMessenger, &AudioMessenger::setDestination);

    connect(_networkSettings, &NetTab::updateDeviceId,
        _audioMessenger, &AudioMessenger::setDeviceId);
    connect(_networkSettings, &NetTab::updateInterfaceIndex,
        _audioMessenger, &AudioMessenger::setInterfaceIndex);
}

void MainWindow::initCallConnections()
{
    connect(_audioMessenger, &AudioMessenger::connectionStatusChange,
        _callSettings, &CallTab::updateConnectionState);

    connect(_audioMessenger, &AudioMessenger::currentNoiseLevel,
        _callSettings, &CallTab::setNoiseLevel);

    connect(_callSettings, &CallTab::updateFrameSize,
        _audioMessenger, &AudioMessenger::setFrameSize);
    connect(_callSettings, &CallTab::updateVolumeLevel,
        _audioMessenger, &AudioMessenger::setVolumeLevel);

    connect(_callSettings, &CallTab::startCall,
        _audioMessenger, &AudioMessenger::startCall);
    connect(_callSettings, &CallTab::applyCall,
        _audioMessenger, &AudioMessenger::applyCall);
    connect(_callSettings, &CallTab::denyCall,
        _audioMessenger, &AudioMessenger::denyCall);
    connect(_callSettings, &CallTab::endCall,
        _audioMessenger, &AudioMessenger::endCall);
}

void MainWindow::initAudioConnections()
{
    connect(_audioMessenger, &AudioMessenger::currentNoiseLevel,
        _audioSettings, &AudioTab::setNoiseLevel);

    connect(_audioMessenger, &AudioMessenger::connectionStatusChange,
        _audioSettings, &AudioTab::updateConnectionState);

    connect(_audioSettings, &AudioTab::updateVadMode,
        _audioMessenger, &AudioMessenger::setVadMode);
    connect(_audioSettings, &AudioTab::updateSelfListeningTestState,
        _audioMessenger, &AudioMessenger::setSelfListeningTestState);

    connect(_audioSettings, &AudioTab::updateInputDevice,
        _audioMessenger, &AudioMessenger::setInputDevice);
    connect(_audioSettings, &AudioTab::updateOutputDevice,
            _audioMessenger, &AudioMessenger::setOutputDevice);
}
