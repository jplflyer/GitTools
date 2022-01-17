TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/GitTool.cpp \
    src/Repository.cpp \
    src/Server.cpp \
    src/Team.cpp \
    src/User.cpp

HEADERS += \
    src/Repository.h \
    src/Server.h \
    src/Team.h \
    src/User.h
