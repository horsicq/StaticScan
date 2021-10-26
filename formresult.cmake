include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/dialogstaticscanprocess.cmake)

set(FORMRESULT_SOURCES
    ${DIALOGSTATICSCANPROCESS_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialogstaticscan.cpp
    ${CMAKE_CURRENT_LIST_DIR}/formresult.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogstaticscan.ui
    ${CMAKE_CURRENT_LIST_DIR}/formresult.ui
)
