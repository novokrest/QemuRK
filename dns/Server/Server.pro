TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11 -pthread
LIBS += -pthread

SOURCES += main.cpp \
    udp.cpp \
    dns.cpp \
    dns_util.cpp


HEADERS += \
    common.h \
    udp.h \
    dns.hpp \
    dns.h \
    dns_util.h
