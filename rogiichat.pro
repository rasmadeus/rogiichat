QT += core gui widgets network

TEMPLATE = app

CONFIG += precompile_header c++14
PRECOMPILED_HEADER = pch.h

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        main_view.cpp \
    client.cpp \
    server.cpp \
    participants_model.cpp \
    participant.cpp

HEADERS += \
        main_view.h \
    client.h \
    server.h \
    participants_model.h \
    participant.h

FORMS += \
        main_view.ui
