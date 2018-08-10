#-------------------------------------------------
#
# Project created by QtCreator 2013-10-27T13:08:03
#
#-------------------------------------------------
QT       += core gui 3d

CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpaceCollisionSystem
TEMPLATE = app

INCLUDEPATH += ../../src/ /usr/local/include/ /usr/local/boost/include/

RESOURCES = resources.qrc

SOURCES += main.cpp \
        collisionsystem.cpp \
    qalgorithm.cpp \
    ../../src/*.cpp \
    collisiondisplay2d.cpp \
    collisiondisplay3d.cpp \
    qparticle2d.cpp \
    collisiondisplay.cpp \
    qparticle3d.cpp

HEADERS  += collisionsystem.h \
    qalgorithm.h \
    ../../src/*.h \
    collisiondisplay2d.h \
    collisiondisplay3d.h \
    qparticle2d.h \
    collisiondisplay.h \
    qparticle3d.h \
    ../../src/spatialindexalgorithm.h \
    ../../src/spatialindexalgorithm3d.h

FORMS    += collisionsystem.ui
