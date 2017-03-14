TEMPLATE = app
CONFIG += c++11
TARGET = Encriptor
QMAKE_CXXFLAGS += -static-libgcc -static-libstdc++ -static

RC_ICONS += "Bez_imeni-1.ico"

QT += core gui widgets

SOURCES += main.cpp \
    filenameedit.cpp \
    face.cpp \
    encriptor.cpp

RESOURCES += \
    res.qrc

HEADERS += \
    filenameedit.h \
    face.h \
    encriptor.h
