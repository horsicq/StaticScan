# TODO guard
include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/staticscan.cmake)

set(DIALOGSTATICSCANPROCESS_SOURCES
    ${STATICSCAN_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialogstaticscanprocess.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogstaticscanprocess.ui
)
