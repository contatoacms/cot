TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    node.cpp \
    job.cpp \
    schedule_flow.cpp \
    file_manager.cpp \
    input_output.cpp \
    simulation.cpp \
    io_file.cpp

HEADERS += \
    node.h \
    job.h \
    schedule_flow.h \
    file_manager.h \
    input_output.h \
    simulation.h \
    io_file.h
