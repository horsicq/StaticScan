INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/dialogstaticscan.h \
    $$PWD/dialogstaticscandirectory.h \
    $$PWD/formresult.h \
    $$PWD/staticscanoptionswidget.h

SOURCES += \
    $$PWD/dialogstaticscan.cpp \
    $$PWD/dialogstaticscandirectory.cpp \
    $$PWD/formresult.cpp \
    $$PWD/staticscanoptionswidget.cpp

FORMS += \
    $$PWD/dialogstaticscan.ui \
    $$PWD/dialogstaticscandirectory.ui \
    $$PWD/formresult.ui \
    $$PWD/staticscanoptionswidget.ui

!contains(XCONFIG, dialogstaticscanprocess) {
    XCONFIG += dialogstaticscanprocess
    include($$PWD/dialogstaticscanprocess.pri)
}

!contains(XCONFIG, dialogtextinfo) {
    XCONFIG += dialogtextinfo
    include($$PWD/../FormatDialogs/dialogtextinfo.pri)
}

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md \
    $$PWD/formresult.cmake
