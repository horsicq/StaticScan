INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/formstaticscan.h

SOURCES += \
    $$PWD/formstaticscan.cpp

FORMS += \
    $$PWD/formstaticscan.ui

!contains(XCONFIG, formresult) {
    XCONFIG += formresult
    include($$PWD/formresult.pri)
}
