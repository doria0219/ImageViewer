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
    logtransformationdlg.cpp \
    spacialfiltercernelinput.cpp \
    gaussblurdialog.cpp \
    bilateralfilterdialog.cpp \
    medianfilterdialog.cpp \
    fft.cpp \
    matrix.cpp

HEADERS  += mainwindow.h \
    mymouseevent.h \
    imageprocessing.h \
    logtransformationdlg.h \
    spacialfiltercernelinput.h \
    gaussblurdialog.h \
    bilateralfilterdialog.h \
    medianfilterdialog.h \
    fft.h \
    matrix.h \
    matrixTemplate.h

FORMS    += mainwindow.ui \
    logtransformationdlg.ui \
    spacialfiltercernelinput.ui \
    gaussblurdialog.ui \
    bilateralfilterdialog.ui \
    medianfilterdialog.ui
