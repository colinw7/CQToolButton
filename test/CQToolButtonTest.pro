TEMPLATE = app

TARGET = 

DEPENDPATH += .

#CONFIG += debug

# Input
SOURCES += \
CQToolButtonTest.cpp \

HEADERS += \
CQToolButtonTest.h \

DESTDIR     = .
OBJECTS_DIR = .

INCLUDEPATH += \
. ../include \
../../CQCheckGrid/include

unix:LIBS += \
-L../lib \
-L../../CQCheckGrid/lib \
-lCQToolButton \
-lCQCheckGrid
