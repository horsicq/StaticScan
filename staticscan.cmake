include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../SpecAbstract/specabstract.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../Formats/scanitem.cmake)

set(STATICSCAN_SOURCES
    ${SPECABSTRACT_SOURCES}
    ${SCANITEM_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/staticscan.cpp
)
