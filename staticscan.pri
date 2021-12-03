INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/staticscan.h

SOURCES += \
    $$PWD/staticscan.cpp

!contains(XCONFIG, specabstract) {
    XCONFIG += specabstract
    include($$PWD/../SpecAbstract/specabstract.pri)
}

!contains(XCONFIG, scanitem) {
    XCONFIG += scanitem
    include($$PWD/../Formats/scanitem.pri)
}

DISTFILES += \
    $$PWD/staticscan.cmake
