#-------------------------------------------------
#
# Project created by QtCreator 2018-02-13T01:25:11
#
#-------------------------------------------------

QT       -= core gui

TARGET = dbcore
TEMPLATE = lib

CONFIG += staticlib #For static lib
CONFIG += c++11

DEFINES += DBCORE_LIBRARY

SOURCES += \
    source/dbcore.cpp \
    sqlite/sqlite3.c \
    cryptopp/blake2.cpp \
    sqlite/dbhash_funct.cpp

HEADERS += \
    include/dbcore.h \
    sqlite/sqlite3.h \
    cryptopp/blake2.h \
    sqlite/dbhash_funct.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
