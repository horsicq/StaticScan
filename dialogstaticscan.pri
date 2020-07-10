INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/dialogstaticscan.h 

SOURCES += \
    $$PWD/dialogstaticscan.cpp 

FORMS += \
    $$PWD/dialogstaticscan.ui 

!contains(XCONFIG, staticscan) {
    XCONFIG += staticscan
    include($$PWD/staticscan.pri)
}
