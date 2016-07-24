# Add more folders to ship with the application, here
folder_01.source = qml/stopwatch-simu
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

QT += quick

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    qtapplicationstopwatch.cpp \
    integration.cpp \
    ../../src/7seg-display.cpp \
    ../../src/clock.cpp \
    ../../src/stopwatchutils.cpp \
    ../../src/stopwatch.cpp \
    ../../src/_main.cpp \
    ../../src/countdown.cpp


# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    qtapplicationstopwatch.h \
    integration.hpp \
    ../../src/7seg-display.hpp \
    ../../src/clock.hpp \
    ../../src/pins.hpp \
    ../../src/stopwatchutils.hpp \
    ../../src/countdown.hpp

DEFINES = QTSIMU
