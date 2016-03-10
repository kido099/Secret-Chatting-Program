#-------------------------------------------------
#
# Project created by QtCreator 2014-11-30T21:11:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test1
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    Clients.cpp \
    MessageBuffer.cpp \
    key_produce.cpp \
    encryption.cpp

HEADERS  += dialog.h \
    Clients.h \
    MessageBuffer.h \
    key_produce.h \
    encryption.h

FORMS    += dialog.ui
