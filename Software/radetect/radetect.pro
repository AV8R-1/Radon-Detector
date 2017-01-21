QT += core
QT -= gui

TARGET = radetect
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    picamera.cpp \
    i2c8274.cpp

HEADERS += \
    picamera.h \
    i2c8274.h

