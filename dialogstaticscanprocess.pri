INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/dialogstaticscanprocess.h 

SOURCES += \
    $$PWD/dialogstaticscanprocess.cpp 

FORMS += \
    $$PWD/dialogstaticscanprocess.ui 

!contains(XCONFIG, staticscan) {
    XCONFIG += staticscan
    include($$PWD/staticscan.pri)
}

!contains(XCONFIG, xoptions) {
    XCONFIG += xoptions
    include($$PWD/../XOptions/xoptions.pri)
}

DISTFILES += \
    $$PWD/dialogstaticscanprocess.cmake
