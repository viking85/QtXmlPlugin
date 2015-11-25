#include "xmlsourceeditor.h"
#include "xmlsourceeditoreditable.h"
#include "xmleditorconstants.h"
#include "xmleditorwidget.h"
#include "xmleditortreeview.h"
#include <QDebug>

using namespace XmlTreeEditor;
using namespace Internal;


XmlSourceEditor::XmlSourceEditor(XmlEditorWidget *widget, QWidget *parent) :
    TextEditor::PlainTextEditor(parent), editorWidget(widget)
{
    XmlSourceDocument *doc = new XmlSourceDocument();
    connect(doc, SIGNAL(saveRequested()), this, SLOT(performPreSave()), Qt::DirectConnection);
    connect(doc, SIGNAL(saveFinished()), editorWidget, SLOT(documentSaved()));
    doc->setMimeType(QLatin1String(XmlTreeEditor::Constants::XML_MIME_TYPE));
    setBaseTextDocument(doc);
    connect(file(), SIGNAL(changed()), this, SLOT(configure()));
}

XmlSourceEditor::~XmlSourceEditor()
{
}

TextEditor::BaseTextEditorEditable *XmlSourceEditor::createEditableInterface()
{
    return new XmlSourceEditorEditable(this, editorWidget);
}

void XmlSourceEditor::performPreSave()
{
    if (editorWidget->currentEditorState() == XmlEditorWidget::TreeViewEditor)
        editorWidget->updateTab(XmlEditorWidget::SourceEditor);
    else
        editorWidget->updateTab(XmlEditorWidget::TreeViewEditor);
}

void XmlSourceEditor::setFontSettings(const TextEditor::FontSettings &fs)
{
    TextEditor::PlainTextEditor::setFontSettings(fs);
    editorWidget->treeView()->setFontSettings(fs);
}


bool XmlSourceDocument::save(const QString &fileName)
{
    emit saveRequested();
    bool result = TextEditor::BaseTextDocument::save(fileName);
    emit saveFinished();
    return result;
}
