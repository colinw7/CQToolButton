TEMPLATE = lib

TARGET = 

DEPENDPATH += .

CONFIG += staticlib

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
