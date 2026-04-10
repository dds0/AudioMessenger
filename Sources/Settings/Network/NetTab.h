#ifndef NETTAB_H
#define NETTAB_H

#include <QWidget>
#include <QHostAddress>
#include <QNetworkInterface>

class SettingsStash;
class NetCommunication;

namespace Ui { class NetTab; }

class NetTab : public QWidget
{
    Q_OBJECT

public:
    explicit NetTab(SettingsStash* stash, QWidget *parent = nullptr);
    ~NetTab();

signals:
    void updateDeviceId(uint8_t deviceId);
    void updateInterfaceIndex(uint index);
    void updateSrc(QHostAddress addr, uint16_t port);
    void updateDst(QHostAddress addr, uint16_t port);

private:
    Ui::NetTab *ui;

    uint16_t _srcPort;
    QHostAddress _srcAddr;

    uint16_t _dstPort;
    QHostAddress _dstAddr;

    bool _settingsApplied{};
    QNetworkInterface _interface;

    SettingsStash* _stash;
    NetCommunication* _netCommunication;

    void initUi();
    void initConnections();

    void saveSettings();
    void applySettings();
    void parseSettings();

    void setupNetworkRules();
    void displayNetworkStatus();
    void updateInterfaceList(const QList<QNetworkInterface>& interfaces);

    void createMessageBox(const QString& text);
    QString createWarningMessage(int warningIndex) const;  
};

#endif // NETTAB_H
