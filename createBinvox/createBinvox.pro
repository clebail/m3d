TEMPLATE = app
TARGET = createBinvox
DEPENDPATH += .
INCLUDEPATH += . ..
QMAKE_CXXFLAGS += -std=c++11

# Input
SOURCES += traite.cpp

HEADERS += ../common.h

