#-------------------------------------------------
#
# Project created by QtCreator 2017-10-24T21:01:31
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simple_solver
TEMPLATE = app

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
        main.cpp \
        mainwindow.cpp \
        reader.cpp \
        model.cpp \
        solver.cpp \
        hdf5_io.cpp

HEADERS += \
        mainwindow.h \
        reader.h \
        model.h \
        solver.h \
        hdf5_io.h

CONFIG += c++11

DEFINES += H5_BUILT_AS_DYNAMIC_LIB

FORMS += \
        mainwindow.ui \

win32: LIBS += -L$$PWD/../hdf5-1.8.18/lib/ -lszip -lzlib -lhdf5 -lhdf5_cpp

INCLUDEPATH += $$PWD/../hdf5-1.8.18/include
DEPENDPATH += $$PWD/../hdf5-1.8.18/include

FORMS += \
        mainwindow.ui

INCLUDEPATH += $$PWD/eigen
