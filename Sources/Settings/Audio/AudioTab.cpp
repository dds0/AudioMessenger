#include "AudioTab.h"
#include "ui_AudioTab.h"

#include <cmath>
#include <cstring>

#include "AudioDeviceManager.h"
#include "Common/SettingsStash.h"

AudioTab::AudioTab(SettingsStash *stash, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudioTab),
    _stash(stash),
    _deviceManager(new AudioDeviceManager(this))
{
    ui->setupUi(this);
    initConnections();
}

AudioTab::~AudioTab()
{
    delete ui;
}

void AudioTab::setNoiseLevel(uint8_t noise)
{
    if (ui->cB_selfListening->isChecked()) {
        ui->hS_microTest->setValue(noise);
    }
}

void AudioTab::updateConnectionState(ConnectionState state)
{
    if (state != Idle)  {
        ui->cB_selfListening->setChecked(false);
    }
    ui->cB_selfListening->setEnabled(state == Idle);
}

void AudioTab::initUi()
{
    _deviceManager->updateList();

    initFilterModes();

    auto vadModeIndex = _stash->getVadModeIndex();
    auto index = ui->cB_noiseFilter->findData(vadModeIndex);

    ui->l_currentVadMode->setText(ui->cB_noiseFilter->itemText(index));

    ui->l_currentInput->setText(_stash->getInputDeviceName());
    ui->l_currentOutput->setText(_stash->getOutputDeviceName());

    emit updateVadMode(AC::VadMode(vadModeIndex));
}

void AudioTab::initFilterModes()
{
    QSignalBlocker _(ui->cB_noiseFilter); Q_UNUSED(_);

    std::map<QString, int> filterModes = {
        {"Normal", AC::VadMode::Normal},
        {"Low bitrate", AC::VadMode::LowBitrate},
        {"Aggressive", AC::VadMode::Aggressive},
        {"Very aggressive", AC::VadMode::VeryAggressive}
    };

    for (auto&& [mode, indexMode] : filterModes) {
        ui->cB_noiseFilter->addItem(mode, indexMode);
    }
}

void AudioTab::initConnections()
{
    initAudioDeviceManagerConnections();

    connect(ui->cB_audioInputs, &QComboBox::currentTextChanged,
        this, &AudioTab::setInputDevice);
    connect(ui->cB_audioOutputs, &QComboBox::currentTextChanged,
        this, &AudioTab::setOutputDevice);

    connect(ui->pB_setNoiseFilter, &QPushButton::clicked,
        this, &AudioTab::setVadMode);

    connect(ui->cB_selfListening, &QCheckBox::stateChanged,
        this, &AudioTab::updateSelfListeningTestState);
    connect(ui->cB_selfListening, &QCheckBox::stateChanged, this,
        [this](auto state){ if (!state) { ui->hS_microTest->setValue(0); } });

    connect(_stash, &SettingsStash::loaded, this, &AudioTab::initUi);
}

void AudioTab::initAudioDeviceManagerConnections()
{
    auto updateDevList = [](auto&& devListWidget, auto&& devList)
    {
        QSignalBlocker _(devListWidget); Q_UNUSED(_);

        devListWidget->clear();
        for (auto&& dev: devList) {
            devListWidget->addItem(dev.deviceName());
        }
    };

    connect(_deviceManager, &AudioDeviceManager::listUpdated, this, [=]() {
        updateDevList(ui->cB_audioInputs, _deviceManager->getInputDevices());
    });

    connect(_deviceManager, &AudioDeviceManager::listUpdated, this, [=]() {
        updateDevList(ui->cB_audioOutputs, _deviceManager->getOutputDevices());
    });

    connect(_deviceManager, &AudioDeviceManager::listUpdated, this, [=]()
    {
        auto inputDev = _deviceManager->getInputDevice(_stash->getInputDeviceName());
        auto outputDev = _deviceManager->getOutputDevice(_stash->getOutputDeviceName());

        emit updateInputDevice(inputDev);
        emit updateOutputDevice(outputDev);
    });
}

void AudioTab::setVadMode()
{
    auto vadIndex = ui->cB_noiseFilter->currentData().toUInt();

    _stash->setVadModeIndex(vadIndex);
    ui->l_currentVadMode->setText(ui->cB_noiseFilter->currentText());

    emit updateVadMode(AC::VadMode(vadIndex));
}

void AudioTab::setInputDevice(QString deviceName)
{
    auto device = _deviceManager->getInputDevice(deviceName);

    _stash->setInputDevice(device);
    ui->l_currentInput->setText(deviceName);

    emit updateInputDevice(device);
}

void AudioTab::setOutputDevice(QString deviceName)
{
    auto device = _deviceManager->getOutputDevice(deviceName);

    _stash->setOutputDevice(device);
    ui->l_currentOutput->setText(deviceName);

    emit updateOutputDevice(device);
}
