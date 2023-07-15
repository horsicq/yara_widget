QT       += concurrent

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/yara_widget.h \
    $$PWD/yarawidgetadvanced.h

SOURCES += \
    $$PWD/yara_widget.cpp \
    $$PWD/yarawidgetadvanced.cpp

FORMS += \
    $$PWD/yara_widget.ui \
    $$PWD/yarawidgetadvanced.ui

contains(XCONFIG, use_yara) {
HEADERS += \
    $$PWD/xyaradialogprocess.h

SOURCES += \
    $$PWD/xyaradialogprocess.cpp
}

!contains(XCONFIG, dialogtextinfo) {
    XCONFIG += dialogtextinfo
    include($$PWD/../FormatDialogs/dialogtextinfo.pri)
}

!contains(XCONFIG, xdialogprocess) {
    XCONFIG += xdialogprocess
    include($$PWD/../FormatDialogs/xdialogprocess.pri)
}

contains(XCONFIG, use_yara) {
    DEFINES += USE_YARA
    !contains(XCONFIG, xyara) {
        XCONFIG += xyara
        include($$PWD/../XYara/xyara.pri)
    }
}

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md \
    $$PWD/yara_widget.cmake
