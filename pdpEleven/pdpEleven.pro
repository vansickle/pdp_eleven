#-------------------------------------------------
#
# Project created by QtCreator 2016-09-28T21:00:06
#
#-------------------------------------------------

QT       -= core gui

TARGET = pdpEleven
TEMPLATE = lib
CONFIG += staticlib
QMAKE_CFLAGS += -std=c99

SOURCES += \
    memory.c \
    asm.c \
    processor.c \
    arraylist.c \
    asmUtils.c \
    dictionary.c \
    asmCommandHandler.c \
    utils.c \
    opcodes.c \
    system.c \
    parserUtils.c

HEADERS += \
    font.h \
    memory.h \
    asm.h \
    processor.h \
    test_program.h \
    utils.h \
    common.h \
    arraylist.h \
    asmConstant.h \
    asmUtils.h \
    dictionary.h \
    opcodes.h \
    asmCommandHandler.h \
    logo.h \
    system.h \
    parserUtils.h
