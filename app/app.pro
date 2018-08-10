#-------------------------------------------------
#
# Project created by QtCreator 2018-07-31T23:26:22
#
#-------------------------------------------------
include(../ft2xx.pri)
QT       += charts multimedia core gui widgets

TARGET = app
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++14

SOURCES += \
	IOTypes/FT2StreamReader.cpp \
	MainWindow.cpp \
	main.cpp \
	IOTypes/FrequencyAnalizerIODevice.cpp \
	IOTypes/FrequencySpectrum.cpp \
	IOTypes/WavFile.cpp \
	IOTypes/XYSeriesIODevice.cpp

HEADERS += \
	IOTypes/FT2StreamReader.hpp \
	MainWindow.hpp \
	IOTypes/FrequencyAnalizerIODevice.hpp \
	IOTypes/FrequencySpectrum.hpp \
	IOTypes/WavFile.hpp \
	IOTypes/XYSeriesIODevice.hpp

FORMS += \
		MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

fftreal_dir = ../3rdparty/fftreal

INCLUDEPATH += $${fftreal_dir}

RESOURCES += \
	spectrum.qrc
