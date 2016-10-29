#-------------------------------------------------
#
# Project created by QtCreator 2016-09-28T21:00:06
#
#-------------------------------------------------

QT       -= core gui

TARGET = pdpEleven
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    pdpeleven.c \
    memory.c \
    asm.c \
    processor.c \
    arraylist.c \
    asmUtils.c \
    asmParseCommand.c

HEADERS += pdpeleven.h \
    font.h \
    memory.h \
    asm.h \
    processor.h \
    test_program.h \
    utils.h \
    common.h \
    arraylist.h \
    asmParseCommand.h \
    asmConstant.h \
    asmUtils.h
