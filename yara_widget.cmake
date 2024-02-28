include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../FormatDialogs/dialogtextinfo.cmake)

set(YARA_WIDGET_SOURCES
    ${FORMRESULT_SOURCES}
    ${DIALOGTEXTINFO_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/yara_widget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/yara_widget.h
    ${CMAKE_CURRENT_LIST_DIR}/yara_widget.ui
    ${CMAKE_CURRENT_LIST_DIR}/yarawidgetadvanced.cpp
    ${CMAKE_CURRENT_LIST_DIR}/yarawidgetadvanced.h
    ${CMAKE_CURRENT_LIST_DIR}/yarawidgetadvanced.ui
    ${CMAKE_CURRENT_LIST_DIR}/xyaradialogprocess.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xyaradialogprocess.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogyarawidgetadvanced.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogyarawidgetadvanced.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogyarawidgetadvanced.ui
)
