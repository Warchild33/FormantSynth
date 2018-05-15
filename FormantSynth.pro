#-------------------------------------------------
#
# Project created by QtCreator 2015-09-02T14:08:08
#
#-------------------------------------------------

QT       += core gui
QT += multimedia
LIBS+=-lqwt-qt5 -lasound
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS+=-g -std=c++11 -Wreorder

TARGET = FormantSynth
TEMPLATE = app

INCLUDEPATH+=/usr/include/qwt

SOURCES += main.cpp\
        mainwindow.cpp \
    stft.cpp \
    istft.cpp \
    pvoc.cpp \
#    psample.cpp \
    fft.cpp \
    wave_in.cpp \
    ploter.cpp \
#    spectrogram.cpp \
    gen.cpp \
    happybirsday.cpp \
    filter.cpp \
    common.cpp \
    test.cpp \
    envelope.cpp \
    alsadriver.cpp \
    sounddriverdialog.cpp \
    formantsyntform.cpp \
    nessyntform.cpp \
    syntezer.cpp \
    formantsynt.cpp \
    pianowidget.cpp \
    freqtable.cpp \
    midinote.cpp \
    key2notetable.cpp \
    buffer.cpp \
    notetowaveformthread.cpp

HEADERS  += mainwindow.h \
    stft.h \
    ploter.h \
    happybirsday.h \
    gen.h \
    filter.h \
    common.h \
    fft.h \
    complex.h \
    wave_in.h \
    pvoc.h \
    test.h \
    envelope.h \
    alsadriver.h \
    sounddriverdialog.h \
    formantsyntform.h \
    nessyntform.h \
    syntezer.h \
    formantsynt.h \
    pianowidget.h \
    freqtable.h \
    midinote.h \
    key2notetable.h \
    buffer.h \
    notetowaveformthread.h
#    spectrogram.h

FORMS    += mainwindow.ui \
    sounddriverdialog.ui \
    formantsyntform.ui \
    nessyntform.ui

OTHER_FILES += \
    play_wav.sh
