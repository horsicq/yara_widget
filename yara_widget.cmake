include_directories(${CMAKE_CURRENT_LIST_DIR})

if (NOT DEFINED DIALOGTEXTINFO_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../FormatDialogs/dialogtextinfo.cmake)
    set(YARA_WIDGET_SOURCES ${YARA_WIDGET_SOURCES} ${DIALOGTEXTINFO_SOURCES})
endif()

set(YARA_WIDGET_SOURCES
    ${YARA_WIDGET_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/yara_widget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/yara_widget.h
    ${CMAKE_CURRENT_LIST_DIR}/yara_widget.ui
    ${CMAKE_CURRENT_LIST_DIR}/yarawidgetadvanced.cpp
    ${CMAKE_CURRENT_LIST_DIR}/yarawidgetadvanced.h
    ${CMAKE_CURRENT_LIST_DIR}/yarawidgetadvanced.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialogyarawidgetadvanced.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogyarawidgetadvanced.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogyarawidgetadvanced.ui
)
