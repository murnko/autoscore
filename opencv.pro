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
CONFIG   += c++11
TEMPLATE = app


SOURCES += main.cpp \
    funkcje.cpp
INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_features2d -lopencv_imgproc
#LIBS     += -lusb-1.0 -lpthread


DISTFILES += \
    komentarz

HEADERS += \
    main.h \
    funkcje.h
