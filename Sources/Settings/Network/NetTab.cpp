#include "NetTab.h"
#include "ui_NetTab.h"
#include <QMessageBox>
#include <QRegExpValidator>

#include "NetCommunication.h"
#include "Common/SettingsStash.h"

const QString StrictRegIPv4 = "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.)"
    "{3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$";

enum NetWarnings {
    interfacelinkDown = 0, notEstablishAddress = 1, notValidAddress = 2
};

NetTab::NetTab(SettingsStash *stash, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetTab),
    _stash(stash),
    _netCommunication(new NetCommunication(this))
{
    ui->setupUi(this);
    initConnections();

    ui->lE_localIPv4->setValidator(
        new QRegExpValidator(QRegExp(StrictRegIPv4), this)
    );
    ui->lE_remoteIPv4->setValidator(
        new QRegExpValidator(QRegExp(StrictRegIPv4), this)
    );
}

NetTab::~NetTab()
{
    delete ui;
}

void NetTab::initUi()
{
    updateInterfaceList(_netCommunication->getInterfaces());

    ui->sB_devId->setValue(_stash->getDeviceId());

    ui->sB_localPort->setValue(_stash->getSrcPort());
    ui->lE_localIPv4->setText(_stash->getSrcAddress().toString());

    ui->sB_remotePort->setValue(_stash->getDstPort());
    ui->lE_remoteIPv4->setText(_stash->getDstAddress().toString());

    auto interfaceIndex = _stash->getInterfaceIndex();
    auto index = ui->cB_possibleIfaces->findData(interfaceIndex);

    if (index != -1) { ui->cB_possibleIfaces->setCurrentIndex(index); }
}

void NetTab::applySettings()
{
    auto strSrcAddr = ui->lE_localIPv4->text();
    auto strDstAddr = ui->lE_remoteIPv4->text();
    auto isSrcAddrAvailable = QRegExp(StrictRegIPv4).exactMatch(strSrcAddr);
    auto isDstAddrAvailable = QRegExp(StrictRegIPv4).exactMatch(strDstAddr);

    if (!isSrcAddrAvailable || !isDstAddrAvailable)
    {
        auto message = createWarningMessage(NetWarnings::notValidAddress);
        createMessageBox(message);
        return;
    }
#ifdef __linux__
    setupNetworkRules();
#endif
    parseSettings();
    saveSettings();

    _settingsApplied = true;
    _netCommunication->updateInterfaces();

    emit updateSrc(_srcAddr, _srcPort);
    emit updateDst(_dstAddr, _dstPort);
    emit updateInterfaceIndex(_interface.index());
}

void NetTab::parseSettings()
{
    _srcPort = ui->sB_localPort->value();
    ui->l_currentLocalPort->setText(QString::number(_srcPort));

    _dstPort = ui->sB_remotePort->value();
    ui->l_currentRemotePort->setText(QString::number(_dstPort));

    _srcAddr = QHostAddress(ui->lE_localIPv4->text());
    ui->l_currentLocalIPv4->setText(_srcAddr.toString());

    _dstAddr = QHostAddress(ui->lE_remoteIPv4->text());
    ui->l_currentRemoteIPv4->setText(_dstAddr.toString());

    auto interfaceIndex = ui->cB_possibleIfaces->currentData().toUInt();

    _interface = _netCommunication->getInterface(interfaceIndex);
    ui->l_currentIface->setText(_interface.humanReadableName());
}

void NetTab::setupNetworkRules()
{
    if (QRegExp(StrictRegIPv4).exactMatch(_srcAddr.toString()))
    {
        if (_dstAddr.isMulticast()) {
            NetCommunication::removeAddress(_srcAddr, _dstAddr);
        }
        NetCommunication::removeAddress(_srcAddr, _interface);
    }

    QHostAddress srcAddr(ui->lE_localIPv4->text());
    QHostAddress dstAddr(ui->lE_remoteIPv4->text());

    auto interfaceIndex = ui->cB_possibleIfaces->currentData().toUInt();
    auto interface = _netCommunication->getInterface(interfaceIndex);

    NetCommunication::appendAddress(srcAddr, interface);
    if (dstAddr.isMulticast()) {
        NetCommunication::appendAddress(srcAddr, dstAddr);
    }
}

void NetTab::createMessageBox(const QString &text)
{
    auto title = tr("Audio Messenger");
    QMessageBox::warning(this, title, text, QMessageBox::Close);
}

void NetTab::displayNetworkStatus()
{
    if (!_settingsApplied) {
        // We should display status only after pressing apply button
        return;
    }

    auto entries = _interface.addressEntries();

    auto sameAddr = [this](auto&& entry) { return entry.ip() == _srcAddr; };
    auto it = std::find_if(entries.begin(), entries.end(), sameAddr);

    auto getIcon = [](auto statement)
    {
        auto style = statement ?
            QStyle::SP_DialogApplyButton : QStyle::SP_MessageBoxWarning;

        return qApp->style()->standardIcon(style);
    };

    auto setStatus = [=](auto&& statusWidget, auto statement, auto netWarning)
    {
        QString okMsg = "Everything is ok";
        auto warningMsg = createWarningMessage(netWarning);

        statusWidget->setIcon(getIcon(statement));
        statusWidget->setToolTip(statement ? okMsg : warningMsg);
    };

    auto addrExist = it != entries.end();
    auto linkIsUp = _interface.flags() & QNetworkInterface::IsUp;

    setStatus(ui->pB_ifaceStatus, linkIsUp, NetWarnings::interfacelinkDown);
    setStatus(ui->pB_AddrStatus, addrExist, NetWarnings::notEstablishAddress);
}

QString NetTab::createWarningMessage(int warningIndex) const
{
    switch (NetWarnings(warningIndex))
    {
        case NetWarnings::interfacelinkDown:
            return tr("Interface link down");
        case NetWarnings::notEstablishAddress:
            return tr("Your don't have src addr on chosen interface");
        case NetWarnings::notValidAddress:
            return tr("You have invalid src/dst addr");
        default:
            break;
    }
    return tr("Undefined warning");
}

void NetTab::initConnections()
{
    connect(ui->pB_applySettings, &QPushButton::clicked,
        this, &NetTab::applySettings);

    connect(_netCommunication, &NetCommunication::interfacesUpdated,
        this, &NetTab::updateInterfaceList);

    connect(_netCommunication, &NetCommunication::interfacesUpdated,
        this, &NetTab::displayNetworkStatus);

    connect(ui->sB_devId, QOverload<int>::of(&QSpinBox::valueChanged),
        this, &NetTab::updateDeviceId);

    connect(_stash, &SettingsStash::loaded, this, &NetTab::initUi);
}

void NetTab::saveSettings()
{
    _stash->setSrcAddress(_srcAddr);
    _stash->setDstAddress(_dstAddr);

    _stash->setSrcPort(_srcPort);
    _stash->setDstPort(_dstPort);
    _stash->setDeviceId(ui->sB_devId->value());

    _stash->setInterfaceIndex(_interface.index());
}

void NetTab::updateInterfaceList(const QList<QNetworkInterface> &interfaces)
{
    auto currentIfaceIndex = ui->cB_possibleIfaces->currentData().toInt();

    ui->cB_possibleIfaces->clear();

    // inserts items into an interface list
    for (auto&& interface: interfaces)
    {
        auto ifaceIndex = interface.index();
        auto ifaceName = interface.humanReadableName();

        ui->cB_possibleIfaces->addItem(ifaceName, ifaceIndex);

        if (currentIfaceIndex == ifaceIndex)
        {
            auto count = ui->cB_possibleIfaces->count();
            ui->cB_possibleIfaces->setCurrentIndex(count - 1);
        }
    }
}
