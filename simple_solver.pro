#-------------------------------------------------
#
# Project created by QtCreator 2017-10-24T21:01:31
#
#-------------------------------------------------

QT       += core gui

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
        graphicsform.cpp

HEADERS += \
        mainwindow.h \
        reader.h \
        model.h \
        solver.h \
        graphicsform.h

INCLUDEPATH += armadillo\include

FORMS += \
        mainwindow.ui \
        graphicsform.ui

win32: LIBS += -L$$PWD/armadillo/lib_win64/ -llapack_win64_MT

INCLUDEPATH += $$PWD/armadillo/lib_win64
DEPENDPATH += $$PWD/armadillo/lib_win64

win32: LIBS += -L$$PWD/armadillo/lib_win64/ -lblas_win64_MT

INCLUDEPATH += $$PWD/armadillo/lib_win64
DEPENDPATH += $$PWD/armadillo/lib_win64


