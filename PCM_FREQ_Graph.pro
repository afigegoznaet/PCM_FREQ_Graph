#-------------------------------------------------
#
# Project created by QtCreator 2018-07-31T23:26:22
#
#-------------------------------------------------

QT       += charts multimedia core gui widgets

TARGET = PCM_FREQ_Graph
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

CONFIG += c++11

SOURCES += \
	app/main.cpp \
	app/MainWindow.cpp \
    app/IOTypes/xyseriesiodevice.cpp \
    app/IOTypes/FT2StreamReader.cpp \
    app/IOTypes/wavfile.cpp

HEADERS += \
	app/MainWindow.hpp \
    app/IOTypes/xyseriesiodevice.hpp \
    app/IOTypes/FT2StreamReader.hpp \
    app/IOTypes/wavfile.h

FORMS += \
		MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
	app/spectrum.qrc
