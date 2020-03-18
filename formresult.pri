INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/formresult.h

SOURCES += \
    $$PWD/formresult.cpp

FORMS += \
    $$PWD/formresult.ui

!contains(XCONFIG, dialogstaticscan) {
    XCONFIG += dialogstaticscan
    include($$PWD/dialogstaticscan.pri)
}



