#-------------------------------------------------
#
# Project created by QtCreator 2015-05-21T02:28:45
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = opencv
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp
INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui
