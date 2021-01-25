#-------------------------------------------------
#
# Project created by QtCreator 2020-02-03T11:20:11
#
#-------------------------------------------------

QT        += core gui

QMAKE_CXXFLAGS += -std=c++0x

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simple-chat
TEMPLATE = app


SOURCES += main.cpp\
        simplechat.cpp \
    tool/for_simple.cpp \
    terminal/net/connect.cpp \
    terminal/mfs/local_msg.cpp

HEADERS  += simplechat.h \
    tool/for_simple.hpp \
    inc/data_type.hpp \
    inc/global_config.hpp \
    terminal/net/connect.hpp \
    terminal/mfs/local_msg.hpp

FORMS    += simplechat.ui

RESOURCES += \

OTHER_FILES += \
    res/images/find.PNG \
    res/system
