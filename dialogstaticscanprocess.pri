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

!contains(XCONFIG, xoptionswidget) {
    XCONFIG += xoptionswidget
    include($$PWD/../XOptions/xoptionswidget.pri)
}

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md \
    $$PWD/dialogstaticscanprocess.cmake
