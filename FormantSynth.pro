#-------------------------------------------------
#
# Project created by QtCreator 2015-09-02T14:08:08
#
#-------------------------------------------------

QT       += core gui svg xml concurrent multimedia
unix: lessThan(QT_MAJOR_VERSION, 5): LIBS+=-lqwt -lasound -lpulse -lpulse-simple
unix: greaterThan(QT_MAJOR_VERSION, 4): LIBS+=-lqwt-qt5 -lasound -lpulse -lpulse-simple

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS+=-g -std=c++11 -Wreorder

TARGET = FormantSynth
TEMPLATE = app

INCLUDEPATH+=/usr/include/qwt
INCLUDEPATH+=./dsp
INCLUDEPATH+=./fft
INCLUDEPATH+=./Formant
INCLUDEPATH+=./Guitar
INCLUDEPATH+=./SyntTest

SOURCES += main.cpp\
        mainwindow.cpp \
    fft/stft.cpp \
    fft/istft.cpp \
    dsp/pvoc.cpp \
#    dsp/psample.cpp \
    fft/fft.cpp \
    qtaudiodriver.cpp \
    wave_in.cpp \
    ploter.cpp \
#    spectrogram.cpp \
    dsp/gen.cpp \
    happybirsday.cpp \
    dsp/filter.cpp \
    common.cpp \
    test.cpp \
    envelope.cpp \
    alsadriver.cpp \
    sounddriverdialog.cpp \
    Formant/formantsyntform.cpp \
    syntezer.cpp \
    Formant/formantsynt.cpp \
    pianowidget.cpp \
    freqtable.cpp \
    midinote.cpp \
    key2notetable.cpp \
    buffer.cpp \
    notetowaveformthread.cpp \
    svgwidget.cpp \
    Formant/formantsynthsvg.cpp \
    svg_path_parser.cpp \
    Formant/mouth.cpp \
    print.cpp \
    Guitar/guitarsynth.cpp \
    SyntTest/synttestform.cpp \
    SyntTest/testsynth.cpp \
    FM/fm.cpp \
    FM/fm_dialog.cpp \
    FM/fm_envelope.cpp \
    channelsdlg.cpp \
    puseaudiodriver.cpp \
    FM/lfo.cpp

HEADERS  += mainwindow.h \
    fft/stft.h \
    ploter.h \
    happybirsday.h \
    dsp/gen.h \
    dsp/filter.h \
    common.h \
    fft/fft.h \
    complex.h \
    qtaudiodriver.h \
    wave_in.h \
    dsp/pvoc.h \
    test.h \
    envelope.h \
    alsadriver.h \
    sounddriverdialog.h \
    Formant/formantsyntform.h \
    syntezer.h \
    Formant/formantsynt.h \
    pianowidget.h \
    freqtable.h \
    midinote.h \
    key2notetable.h \
    buffer.h \
    notetowaveformthread.h \
    svgwidget.h \
    Formant/formantsynthsvg.h \
    svg_path_parser.h \
    Formant/mouth.h \
    print.h \
    Guitar/guitarsynth.h \
    SyntTest/synttestform.h \
    SyntTest/testsynth.h \
    fft/istft.h \
    FM/fm_dialog.h \
    FM/fm.h \
    FM/fm_envelope.h \
    channelsdlg.h \
    puseaudiodriver.h \
    FM/lfo.h
#    spectrogram.h

FORMS    += mainwindow.ui \
    sounddriverdialog.ui \
    Formant/formantsyntform.ui \
    SyntTest/nessyntform.ui \
    FM/fm_dialog.ui \
    channelsdlg.ui

OTHER_FILES += \
    play_wav.sh \
    midi_data/happy_birsday.txt

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/3PartyLibs/lib/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/3PartyLibs/lib/ -lqwtd

INCLUDEPATH += $$PWD/3PartyLibs/qwt
DEPENDPATH += $$PWD/3PartyLibs/qwt

INCLUDEPATH += $$PWD/3PartyLibs
DEPENDPATH += $$PWD/3PartyLibs

