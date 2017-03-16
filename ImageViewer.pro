#-------------------------------------------------
#
# Project created by QtCreator 2017-03-02T13:38:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageViewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mymouseevent.cpp \
    imageprocessing.cpp \
    logtransformationdlg.cpp

HEADERS  += mainwindow.h \
    mymouseevent.h \
    imageprocessing.h \
    logtransformationdlg.h

FORMS    += mainwindow.ui \
    logtransformationdlg.ui
