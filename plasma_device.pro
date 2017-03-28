TEMPLATE = app
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = plasma_device
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += LeptonThread.h mainwindow.h
SOURCES += LeptonThread.cpp main.cpp mainwindow.cpp

# unix: LIBS += -LleptonSDKEmb32PUB/Debug -lLEPTON_SDK
