INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/dialogstaticscanprocess.h 

SOURCES += \
    $$PWD/dialogstaticscanprocess.cpp 

!contains(XCONFIG, staticscan) {
    XCONFIG += staticscan
    include($$PWD/staticscan.pri)
}

!contains(XCONFIG, xoptionswidget) {
    XCONFIG += xoptionswidget
    include($$PWD/../XOptions/xoptionswidget.pri)
}

!contains(XCONFIG, xdialogprocess) {
    XCONFIG += xdialogprocess
    include($$PWD/../FormatDialogs/xdialogprocess.pri)
}

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md \
    $$PWD/dialogstaticscanprocess.cmake
