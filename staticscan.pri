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

!contains(XCONFIG, qpe) {
    XCONFIG += qpe
    include(../Formats/qpe.pri)
}

!contains(XCONFIG, specabstract) {
    XCONFIG += specabstract
    include(../Formats/spec/specabstract.pri)
}
