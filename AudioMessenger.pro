QT += core gui widgets network multimedia
CONFIG += c++17

INCLUDEPATH += $$PWD/
INCLUDEPATH += $$PWD/Sources

QMAKE_CXXFLAGS = -std=c++17 -Wall -Wshadow -Wpointer-arith -Wcast-qual -pedantic

DEFINES += "TAG='\"$$system(cd $$_PRO_FILE_PWD_ && git describe --tags --always)\"'"

include($$PWD/WebRtcAudio/WebRtcAudio.pri)

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/Sources/mainwindow.cpp \
    $$PWD/Sources/Common/SettingsStash.cpp \
    $$PWD/Sources/Network/Proto/AMCDP.cpp \
    $$PWD/Sources/Network/Proto/AMCDP_Client.cpp \
    $$PWD/Sources/Settings/Network/NetTab.cpp \
    $$PWD/Sources/Settings/Network/NetCommunication.cpp \
    $$PWD/Sources/Settings/Call/CallTab.cpp \
    $$PWD/Sources/Settings/Audio/AudioTab.cpp \
    $$PWD/Sources/Settings/Audio/AudioDeviceManager.cpp \
    $$PWD/Sources/Audio/Utils/AudioConstants.cpp \
    $$PWD/Sources/Audio/Utils/AudioQueue.cpp \
    $$PWD/Sources/Audio/AudioMessenger.cpp \
    $$PWD/Sources/Audio/ConnectionLogic.cpp \
    $$PWD/Sources/Audio/Output/AudioMixer.cpp \
    $$PWD/Sources/Audio/Output/AudioOutput.cpp \
    $$PWD/Sources/Audio/Output/SoundEffects.cpp \
    $$PWD/Sources/Audio/Input/AudioInput.cpp \
    $$PWD/Sources/Audio/Input/AudioSwitch.cpp \
    $$PWD/Sources/Audio/Input/PacketConverter.cpp \
    $$PWD/Sources/Audio/Input/VoiceAudioDetector.cpp \
    $$PWD/Sources/Audio/Input/SelfListeningSwitch.cpp

HEADERS += \
    $$PWD/Sources/mainwindow.h \
    $$PWD/Sources/Common/SettingsStash.h \
    $$PWD/Sources/Settings/Network/NetTab.h \
    $$PWD/Sources/Settings/Network/NetCommunication.h \
    $$PWD/Sources/Settings/Call/CallTab.h \
    $$PWD/Sources/Settings/Audio/AudioTab.h \
    $$PWD/Sources/Settings/Audio/AudioDeviceManager.h \
    $$PWD/Sources/Network/Proto/AMCDP.h \
    $$PWD/Sources/Network/Proto/AMCDPSpace.h \
    $$PWD/Sources/Network/Proto/AMCDP_Client.h \
    $$PWD/Sources/Audio/Utils/AudioQueue.h \
    $$PWD/Sources/Audio/Utils/AudioConstants.h \
    $$PWD/Sources/Audio/Utils/ConnectionState.h \
    $$PWD/Sources/Audio/AudioMessenger.h \
    $$PWD/Sources/Audio/ConnectionLogic.h \
    $$PWD/Sources/Audio/Output/AudioMixer.h \
    $$PWD/Sources/Audio/Output/AudioOutput.h \
    $$PWD/Sources/Audio/Input/AudioInput.h \
    $$PWD/Sources/Audio/Input/AudioSwitch.h \
    $$PWD/Sources/Audio/Input/PacketConverter.h \
    $$PWD/Sources/Audio/Output/SoundEffects.h \
    $$PWD/Sources/Audio/Input/VoiceAudioDetector.h \
    $$PWD/Sources/Audio/Input/SelfListeningSwitch.h

FORMS += \
    $$PWD/Sources/mainwindow.ui \
    $$PWD/Sources/Settings/Call/CallTab.ui \
    $$PWD/Sources/Settings/Network/NetTab.ui \
    $$PWD/Sources/Settings/Audio/AudioTab.ui

TRANSLATIONS += \
    $$PWD/Qrc/translations/EN.ts

RESOURCES += \
    $$PWD/Qrc/Resourses.qrc
