TEMPLATE = lib
TARGET = nfcshareplugin
CONFIG += plugin link_pkgconfig
PKGCONFIG += nemotransferengine-qt5
QT += dbus
QT -= gui

include(../config.pri)

QMAKE_CXXFLAGS += -Wno-unused-parameter -fvisibility=hidden
QMAKE_CFLAGS += -fvisibility=hidden
QMAKE_LFLAGS += -fvisibility=hidden

INCLUDEPATH += \
    ../shareplugin

DEFINES += \
    NFCSHARE_UI_DIR=\\\"$$NFCSHARE_UI_DIR\\\" \
    NFCSHARE_UI_FILE=\\\"$$NFCSHARE_UI_FILE\\\"

CONFIG(debug, debug|release) {
    DEFINES += DEBUG
}

CONFIG(use_svg) {
    DEFINES += USE_SVG
}

SOURCES += \
    src/nfcshareplugin.cpp

UI_FILES = \
    qml/$${NFCSHARE_UI_FILE}

OTHER_FILES += $$UI_FILES

nfcshare_ui.files = $${UI_FILES}
nfcshare_ui.path = $${NFCSHARE_UI_DIR}
INSTALLS += nfcshare_ui

target.path = $$[QT_INSTALL_LIBS]/nemo-transferengine/plugins/sharing
INSTALLS += target
