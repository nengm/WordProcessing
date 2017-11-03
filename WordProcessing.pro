#-------------------------------------------------
#
# Project created by QtCreator 2017-10-27T11:06:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WordProcessing
TEMPLATE = app


SOURCES += main.cpp \
    WordProcessingForm.cpp \
    WordDrawQWidget.cpp \
    WordProcess.cpp

HEADERS  += \
    WordProcessingForm.h \
    WordDrawQWidget.h \
    WordProcess.h

FORMS    +=
QMAKE_CXXFLAGS +=  -Wno-unused-parameter
