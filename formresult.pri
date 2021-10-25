INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/dialogstaticscan.h \
    $$PWD/formresult.h

SOURCES += \
    $$PWD/dialogstaticscan.cpp \
    $$PWD/formresult.cpp

FORMS += \
    $$PWD/dialogstaticscan.ui \
    $$PWD/formresult.ui

!contains(XCONFIG, dialogstaticscanprocess) {
    XCONFIG += dialogstaticscanprocess
    include($$PWD/dialogstaticscanprocess.pri)
}

DISTFILES += \
    $$PWD/formresult.cmake
