#-------------------------------------------------
#
# Project created by QtCreator 2012-12-25T21:16:48
#
#-------------------------------------------------

QT       += core gui Logic

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = auto-rotate-screen
TEMPLATE = app


SOURCES +=\
    WinOSFunctions.cpp \
    WinOSApsSensor.cpp \
    Settings.cpp \
    DiskStoring.cpp \
    Logic.cpp \
    Main.cpp \
    MainWindow.cpp \
    HelpDialog.cpp

HEADERS  += \
    WinOSFunctions.h \
    WinOSApsSensor.h \
    Settings.h \
    DiskStoring.h \
    Logic.h \
    MainWindow.h \
    OrientSettings.h \
    SensorData.h \
    HelpDialog.h \
    constants.h

FORMS    += mainwindow.ui \
    HelpDialog.ui
QMAKE_CXXFLAGS += -pedantic -std=c++11 -Wall -Wextra -Wmain

RESOURCES += \
    resource.qrc

QTPLUGIN     += qico

RC_FILE = resources.rc


