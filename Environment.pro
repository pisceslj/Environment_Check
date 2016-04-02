#-------------------------------------------------
#
# Project created by QtCreator 2016-01-31T17:00:26
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Environment
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    qextserialbase.cpp \
    qextserialenumerator.cpp \
    qextserialport.cpp \
    win_qextserialport.cpp \
    WaveScene.cpp \
    dialog.cpp \
    mytablemodel.cpp

HEADERS  += widget.h \
    qextserialbase.h \
    qextserialenumerator.h \
    qextserialport.h \
    win_qextserialport.h \
    WaveScene.h \
    dialog.h \
    mytablemodel.h

FORMS    += widget.ui \
    dialog.ui
