#-------------------------------------------------
#
# Project created by QtCreator 2017-12-02T14:21:35
#
#-------------------------------------------------

QT       -= gui
QT       += widgets

TARGET = lib_simple_solver
TEMPLATE = lib
CONFIG += dll
DESTDIR = $$PWD

DEFINES += LIB_SIMPLE_SOLVER_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        lib_simple_solver.cpp \
        reader.cpp \
        solver.cpp \
        model.cpp \
        logger.cpp \
        hdf5_io.cpp


HEADERS += \
        lib_simple_solver.h \
        lib_simple_solver_global.h \ 
        reader.h \
        solver.h \
        model.h \
        logger.h \
        interpol.h \
        hdf5_io.h

CONFIG += c++11

DEFINES += H5_BUILT_AS_DYNAMIC_LIB

win32: LIBS += -L$$PWD/../hdf5-1.8.18/lib/ -lszip -lzlib -lhdf5 -lhdf5_cpp

INCLUDEPATH += $$PWD/../hdf5-1.8.18/include
DEPENDPATH += $$PWD/../hdf5-1.8.18/include

INCLUDEPATH += $$PWD/eigen
INCLUDEPATH += $$PWD/unsupported

INCLUDEPATH += $$PWD/highfive/include


unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    interface.py
