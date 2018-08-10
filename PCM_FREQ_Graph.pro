include(ft2xx.pri)

TARGET = PCM_FREQ_Graph
TEMPLATE = subdirs

# Ensure that library is built before application
CONFIG  += ordered

SUBDIRS += 3rdparty/fftreal
SUBDIRS += app

app.depends = 3rdparty/fftreal

