include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/staticscan.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../XOptions/xoptionswidget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../FormatDialogs/xdialogprocess.cmake)

set(DIALOGSTATICSCANPROCESS_SOURCES
    ${STATICSCAN_SOURCES}
    ${XOPTIONSWIDGET_SOURCES}
    ${XDIALOGPROCESS_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialogstaticscanprocess.cpp
)
