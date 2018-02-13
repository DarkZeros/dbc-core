#-------------------------------------------------
#
# Project created by QtCreator 2018-02-13T01:25:11
#
#-------------------------------------------------

QT       -= core gui

TARGET = dbcore
TEMPLATE = lib

DEFINES += DBCORE_LIBRARY

SOURCES += \
    source/dbcore.cpp \
    sqlite/sqlite3.c

HEADERS += \
    include/dbcore.h \
    sqlite/sqlite3.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
