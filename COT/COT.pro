TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    node.cpp \
    job.cpp \
    schedule_flow.cpp \
    file_manager.cpp

HEADERS += \
    node.h \
    job.h \
    schedule_flow.h \
    file_manager.h
