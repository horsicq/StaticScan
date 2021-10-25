INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/heurwidget.h

SOURCES += \
    $$PWD/heurwidget.cpp

FORMS += \
    $$PWD/heurwidget.ui

!contains(XCONFIG, formresult) {
    XCONFIG += formresult
    include($$PWD/formresult.pri)
}

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md \
    $$PWD/heurwidget.cmake
