include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/formresult.cmake)

set(HEURWIDGET_SOURCES
    ${FORMRESULT_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/heurwidget.cpp
)
