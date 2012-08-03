CONFIG      += designer plugin debug_and_release
TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib
QTDIR_build:DESTDIR     = $$QT_BUILD_TREE/plugins/designer

HEADERS     = qled.h \
              qledplugin.h
SOURCES     = qled.cpp \
              qledplugin.cpp

RESOURCES = qled.qrc

# install
target.path = $$[QT_INSTALL_PLUGINS]/designer
sources.files = $$SOURCES $$HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/designer/qledplugin
INSTALLS += target sources
