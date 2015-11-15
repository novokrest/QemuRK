TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += c++11

SOURCES += \
    common/cryptographer.cpp \
    common/dns_util.cpp \
    common/ip.cpp \
    common/messages.cpp \
    common/udp.cpp \
    common/dns.c \
    common/util.c \
    ksniffer/dns_sender.cpp \
    ksniffer/key_sniffer.cpp \
    ksniffer/key_stat_collector.cpp \
    server/main.cpp \
    server/server.cpp \
    common/dns_ex.cpp

HEADERS += \
    common/common.hpp \
    common/cryptographer.hpp \
    common/defines.h \
    common/dns.h \
    common/dns_util.h \
    common/ip.hpp \
    common/messages.hpp \
    common/udp.hpp \
    common/util.h \
    ksniffer/dns_sender.hpp \
    ksniffer/key_sniffer.hpp \
    ksniffer/key_stat_collector.hpp \
    server/server.hpp \
    common/dns_ex.hpp

INCLUDEPATH += tomcrypt/headers
LIBS += -L./tomcrypt/libs -ltomcrypt -ltfm
