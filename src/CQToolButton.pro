TEMPLATE = lib

QT += widgets

TARGET = CQToolButton

DEPENDPATH += .

QMAKE_CXXFLAGS += -std=c++17

CONFIG += staticlib

QT += widgets

# Input
HEADERS += \
../include/CQToolButton.h \

SOURCES += \
CQToolButton.cpp \

OBJECTS_DIR = ../obj

DESTDIR = ../lib

INCLUDEPATH += \
. \
../include \
