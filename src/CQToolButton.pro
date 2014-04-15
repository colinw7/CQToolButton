TEMPLATE = lib

TARGET = CQToolButton

DEPENDPATH += .

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
