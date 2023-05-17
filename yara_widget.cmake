include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../FormatDialogs/dialogtextinfo.cmake)

set(YARA_WIDGET_SOURCES
    ${FORMRESULT_SOURCES}
    ${DIALOGTEXTINFO_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/yara_widget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/yara_widget.ui
)
