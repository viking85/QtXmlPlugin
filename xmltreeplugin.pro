TARGET = XmlTreePlugin
QT += xml
TEMPLATE = lib

DEFINES += XMLTREEPLUGIN_LIBRARY

# XmlTreePlugin files

SOURCES += xmltreeplugin.cpp \
    xmleditorfactory.cpp \
    xmltreemodel.cpp \
    xmltreenamedelegate.cpp \
    xmleditortreeview.cpp \
    xmleditorwidget.cpp \
    xmlsourceeditor.cpp \
    xmlsourceeditoreditable.cpp \
    xmltreevaluedelegate.cpp

HEADERS += xmltreeplugin.h\
        xmltreeplugin_global.h \
    xmleditorfactory.h \
    xmltreemodel.h \
    xmltreenamedelegate.h \
    xmleditortreeview.h \
    xmleditorwidget.h \
    xmlsourceeditor.h \
    xmleditorconstants.h \
    xmlsourceeditoreditable.h \
    xmltreevaluedelegate.h

OTHER_FILES =


# Qt Creator linking

## set the QTC_SOURCE environment variable to override the setting here
QTCREATOR_SOURCES = $$(QTC_SOURCE)
isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=

## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=

PROVIDER = BlackTass

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)
include($$QTCREATOR_SOURCES/src/plugins/coreplugin/coreplugin.pri)
include($$QTCREATOR_SOURCES/src/plugins/texteditor/texteditor.pri)

LIBS += -L$$IDE_PLUGIN_PATH/Nokia
