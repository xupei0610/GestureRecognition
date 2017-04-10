#-------------------------------------------------
#
# Project created by QtCreator 2017-03-28T15:21:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GestureRecognition
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += src/main.cpp\
        src/MainView.cpp \
    src/MySettings.cpp \
    src/HandDetector.cpp \
    src/SettingView.cpp \
    src/MonitorView.cpp \
    src/TrackingView.cpp \
    src/GestureAnalyst.cpp \
    src/CommandInputter.cpp \
    src/SampleCollector.cpp \
    src/GestureControlSystem.cpp \
    src/ImgConvertor.cpp

HEADERS  += src/MainView.h \
    src/HandDetector.h \
    src/SettingView.h \
    src/MonitorView.h \
    src/TrackingView.h \
    src/global.h \
    src/Singleton.h \
    src/SampleCollector.h \
    src/ImgConvertor.h \
    src/Settings.h \
    src/GestureControlSystem.h \
    src/CommandInputterInterface.h \
    src/CommandInputter.h \
    src/GestureAnalystInterface.h \
    src/GestureAnalyst.h



INCLUDEPATH += /usr/local/cellar/lmdb/0.9.19/include
LIBS += -L/usr/local/cellar/lmdb/0.9.19/lib -llmdb

INCLUDEPATH += /usr/local/cellar/boost/1.63.0/include
LIBS += -L/usr/local/cellar/boost/1.63.0/lib -lboost_system

INCLUDEPATH += /usr/local/include                   # opencv, gflags or maybe others
LIBS += -L/usr/local/lib -lopencv_videoio -lopencv_video -lopencv_imgproc -lopencv_core -lopencv_imgcodecs

INCLUDEPATH += /usr/local/cuda/include
INCLUDEPATH += /usr/local/cellar/openblas/0.2.18_2/include

INCLUDEPATH += /usr/local/cellar/glog/0.3.4_1/include
LIBS += -L/usr/local/cellar/glog/0.3.4_1/lib -lglog

INCLUDEPATH += /usr/local/cellar/protobuf/3.2.0/include
LIBS += -L/usr/local/cellar/protobuf/3.2.0/lib -lprotobuf

INCLUDEPATH += /Users/XP/Downloads/caffe-master/include
QMAKE_RPATHDIR += /Users/XP/Downloads/caffe-master/build/lib
LIBS += -L/Users/XP/Downloads/caffe-master/build/lib -lcaffe


LIBS += -framework ApplicationServices
