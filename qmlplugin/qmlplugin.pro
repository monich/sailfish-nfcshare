TEMPLATE = lib
TARGET = nfcshareqmlplugin
CONFIG += plugin link_pkgconfig
PKGCONFIG += libnfcdef
QT += dbus qml

QMAKE_CXXFLAGS += -Wno-unused-parameter -fvisibility=hidden
QMAKE_LFLAGS += -fvisibility=hidden

CONFIG(debug, debug|release) {
    DEFINES += DEBUG
}

include(../config.pri)

HEADERS += \
    ndefapp.h \
    nfcshare.h

SOURCES += \
    ndefapp.cpp \
    nfcshare.cpp \
    plugin.cpp

OTHER_FILES += \
    qmldir

TARGET_PATH = $$[QT_INSTALL_QML]/org/sailfishos/nfcshare

target.path = $${TARGET_PATH}
INSTALLS += target

qmldir.path = $${TARGET_PATH}
qmldir.files += qmldir
INSTALLS += qmldir
