INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/staticscan.h \
    $$PWD/staticscanitem.h \
    $$PWD/staticscanitemmodel.h

SOURCES += \
    $$PWD/staticscan.cpp \
    $$PWD/staticscanitem.cpp \
    $$PWD/staticscanitemmodel.cpp

!contains(XCONFIG, specabstract) {
    XCONFIG += specabstract
    include($$PWD/../SpecAbstract/specabstract.pri)
}

DISTFILES += \
    $$PWD/staticscan.cmake
