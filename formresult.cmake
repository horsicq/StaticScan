include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/dialogstaticscanprocess.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../FormatDialogs/dialogtextinfo.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../XShortcuts/xshortcuts.cmake)

set(FORMRESULT_SOURCES
    ${DIALOGSTATICSCANPROCESS_SOURCES}
    ${DIALOGTEXTINFO_SOURCES}
    ${XSHORTCUTS_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialogstaticscan.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogstaticscan.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialogstaticscandirectory.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogstaticscandirectory.ui
    ${CMAKE_CURRENT_LIST_DIR}/formresult.cpp
    ${CMAKE_CURRENT_LIST_DIR}/formresult.ui
    ${CMAKE_CURRENT_LIST_DIR}/staticscanoptionswidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/staticscanoptionswidget.ui
)
