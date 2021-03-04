
TEMPLATE = lib
CONFIG += plugin
QT += widgets
INCLUDEPATH += ../app

TARGET          = $$qtLibraryTarget(calplugin)
DESTDIR         = ../plugins

HEADERS += \
    calplugin.h

SOURCES += \
    calplugin.cpp

DISTFILES += \
    calplugin.json
