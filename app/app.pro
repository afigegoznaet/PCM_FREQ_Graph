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
	IOTypes/xyseriesiodevice.cpp \
	IOTypes/FT2StreamReader.cpp \
	IOTypes/wavfile.cpp \
	MainWindow.cpp \
	main.cpp

HEADERS += \
	IOTypes/xyseriesiodevice.hpp \
	IOTypes/FT2StreamReader.hpp \
	IOTypes/wavfile.h \
	MainWindow.hpp

FORMS += \
		MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
	spectrum.qrc
