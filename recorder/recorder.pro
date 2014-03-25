TEMPLATE = lib
QT += qml quick multimedia
CONFIG += plugin

DESTDIR = Recorder
TARGET = recorderplugin

OBJECTS_DIR = tmp
MOC_DIR = tmp

# Input
SOURCES += \
    recorder_plugin.cpp \
    audiorecorder.cpp \
    utils.cpp

HEADERS += \
    recorder_plugin.h \
    audiorecorder.h \
    utils.h
