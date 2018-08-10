#-------------------------------------------------
#
# Project created by QtCreator 2014-01-28T22:56:16
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = SpaceCollisionSystemConsole
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../src/ /usr/local/include/ /usr/local/boost/include/

SOURCES += main.cpp \
    ../../src/*.cpp

HEADERS += ../../src/*.h
