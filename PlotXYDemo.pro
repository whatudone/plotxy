# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = app
TARGET = SimDataAnalyzer
QT += core gui widgets printsupport network
CONFIG += c++11
LIBS += -luser32
win32-msvc {
    QMAKE_CXXFLAGS += /std:c++11
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
    DEFINES += NOMINMAX
    INCLUDEPATH += $$PWD/third_party/protobuf/msvc2015_64/include
    CONFIG(debug,debug|release) {
        LIBS+= -L$$PWD/third_party/protobuf/msvc2015_64/lib -llibprotobufd
        QMAKE_CFLAGS_DEBUG += -MTd
        QMAKE_CXXFLAGS_DEBUG += -MTd
    } else {
        LIBS+= -L$$PWD/third_party/protobuf/msvc2015_64/lib -llibprotobuf
        QMAKE_CFLAGS_RELEASE += -MT
        QMAKE_CXXFLAGS_RELEASE += -MT
    }
    DEFINES+= H5_BUILT_AS_DYNAMIC_LIB
    INCLUDEPATH += $$PWD/third_party/hdf5/include
    LIBS += -L$$PWD/third_party/hdf5/lib -lhdf5_cpp -lhdf5
}

win32-g++{
    QMAKE_CXXFLAGS += -std=c++11
    INCLUDEPATH += $$PWD/third_party/protobuf/mingw53_32/include
    LIBS+= -L$$PWD/third_party/protobuf/mingw53_32/lib -lprotobuf.dll
}
RC_ICONS = logo.ico
include(PlotXYDemo.pri)
