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
    sqlite/sqlite3.c \
    sqlite/sha3sum.cpp \
    cryptopp/blake2.cpp \
    source/utils.cpp \
    source/timer.cpp \
    sqlite/sqlite_util.cpp \
    source/config.cpp \
    source/core.cpp \
    source/core.cpp \
    protobuf/client.pb.cc \
    source/p2p.cpp \
    source/logger.cpp

HEADERS += \
    sqlite/sqlite3.h \
    sqlite/sha3sum.h \
    cryptopp/blake2.h \
    source/utils.h \
    source/timer.h \
    sqlite/sqlite_util.h \
    source/config.h \
    source/core.h \
    protobuf/client.pb.h \
    source/p2p.h \
    source/logger.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    protobuf/client.proto
