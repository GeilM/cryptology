#-------------------------------------------------
#
# Project created by QtCreator 2016-12-15T23:39:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MesgConfirm
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    _sha1.cpp \
    rsa.cpp

HEADERS  += mainwindow.h \
    _sha1.h \
    rsa.h

FORMS    += mainwindow.ui
