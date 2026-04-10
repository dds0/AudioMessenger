#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class NetTab;
class CallTab;
class AudioTab;

class SettingsStash;
class AudioMessenger;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    SettingsStash* _stash;

    CallTab* _callSettings;
    NetTab* _networkSettings;
    AudioTab* _audioSettings;

    AudioMessenger* _audioMessenger;

    void initNetConnections();
    void initCallConnections();
    void initAudioConnections();
};
#endif // MAINWINDOW_H
