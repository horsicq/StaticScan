INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/staticscan.h

SOURCES += \
    $$PWD/staticscan.cpp

!contains(XCONFIG, qpe) {
    XCONFIG += qpe
    include(../Formats/qpe.pri)
}

!contains(XCONFIG, specabstract) {
    XCONFIG += specabstract
    include(../Formats/spec/specabstract.pri)
}
