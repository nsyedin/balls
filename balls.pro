#-------------------------------------------------
#
# Project created by QtCreator 2017-10-13T12:15:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = balls
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ball.cpp \
    scene.cpp

HEADERS  += mainwindow.h \
    ball.h \
    scene.h

FORMS    += mainwindow.ui
