# Qt 5.15.0
QT       += core

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle


INCLUDEPATH += $$PWD/externals\opencv_3.4.16\build\include


debug {
    LIBS += $$PWD/externals\opencv_3.4.16\build\x64\vc15\lib\opencv_world3416d.lib
}

release {
    LIBS += $$PWD/externals\opencv_3.4.16\build\x64\vc15\lib\opencv_world3416.lib
}


SOURCES += \
        sources/src/main.cpp \
        sources/src/detection.cpp \
        sources/src/utils.cpp \
        sources/src/apphelper.cpp

HEADERS += \
        sources/inc/detection.h \
        sources/inc/utils.h \
        sources/inc/apphelper.h
