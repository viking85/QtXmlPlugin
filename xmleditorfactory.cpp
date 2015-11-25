#include "xmleditorfactory.h"
#include "xmltreeplugin.h"
#include "xmleditorwidget.h"
#include "xmlsourceeditor.h"
#include "xmleditorconstants.h"

#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/editormanager/ieditor.h>
#include <texteditor/texteditorsettings.h>

using namespace XmlTreeEditor;
using namespace XmlTreeEditor::Internal;


namespace XmlTreeEditor {
namespace Internal {

class XmlEditorFactoryPrivate
{
public:
    QString id;
    QStringList mimeTypes;
    XmlEditorFactory *q;
};

}
}


XmlEditorFactory::XmlEditorFactory(XmlTreeEditor::Internal::XmlTreePluginImpl *owner) :
    Core::IEditorFactory(owner), d(new XmlEditorFactoryPrivate)
{
    d->q = this;
    d->id = Constants::XMLTREEEDITOR_NAME;
    d->mimeTypes << Constants::XML_MIME_TYPE;
}

XmlEditorFactory::~XmlEditorFactory()
{
    delete d;
}


QStringList XmlEditorFactory::mimeTypes() const
{
    return d->mimeTypes;
}

QString XmlEditorFactory::id() const
{
    return d->id;
}

QString XmlEditorFactory::displayName() const
{
    return Constants::XMLTREEEDITOR_NAME;

}

Core::IFile *XmlEditorFactory::open(const QString &fileName)
{
    Core::IEditor *iface = Core::EditorManager::instance()->openEditor(fileName, id());
    return iface ? iface->file() : 0;
}

Core::IEditor *XmlEditorFactory::createEditor(QWidget *parent)
{
    XmlEditorWidget *editorWidget = new XmlEditorWidget(parent);
    TextEditor::TextEditorSettings::instance()->initializeEditor(editorWidget->sourceEditor());

    disconnect(TextEditor::TextEditorSettings::instance(),
               SIGNAL(fontSettingsChanged(TextEditor::FontSettings)),
               editorWidget->sourceEditor(), 0);
    connect(TextEditor::TextEditorSettings::instance(),
            SIGNAL(fontSettingsChanged(TextEditor::FontSettings)),
            editorWidget->sourceEditor(),
            SLOT(setFontSettings(TextEditor::FontSettings)));

    return editorWidget->sourceEditor()->editableInterface();
}
