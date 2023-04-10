TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/local/include

SOURCES += \
    src/BranchProtection.cpp \
    src/GitTool.cpp \
    src/Repository.cpp \
    src/Server.cpp \
    src/Team.cpp \
    src/User.cpp

HEADERS += \
    src/BranchProtection.h \
    src/Repository.h \
    src/Server.h \
    src/Team.h \
    src/User.h
