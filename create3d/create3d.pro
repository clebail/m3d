TEMPLATE = app
TARGET = create3d
LIBS += -lSDL -lGL -lGLU
DEPENDPATH += .
INCLUDEPATH += . .. /usr/include/SDL

# Input
SOURCES += traite.cpp

HEADERS += ../common.h
