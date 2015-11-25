#include "xmlsourceeditoreditable.h"
#include "xmlsourceeditor.h"
#include "xmleditorwidget.h"
#include "xmleditortreeview.h"
#include "xmleditorconstants.h"

#include <QDebug>

using namespace XmlTreeEditor;

XmlSourceEditorEditable::XmlSourceEditorEditable(XmlSourceEditor *editor, XmlEditorWidget *widget)
  : TextEditor::PlainTextEditorEditable(editor), m_context(Constants::XMLTREEEDITOR_ID, Constants::XMLTREEEDITOR_NAME), editorWidget(widget)
{
    connect(editorWidget->treeView(), SIGNAL(contentModified()), this, SIGNAL(contentsChanged()));
}

XmlSourceEditorEditable::~XmlSourceEditorEditable()
{
    delete editorWidget;
}

Core::Context XmlSourceEditorEditable::context() const
{
    return m_context;
}

QString XmlSourceEditorEditable::id() const
{
    return QLatin1String(Constants::XMLTREEEDITOR_ID);
}

QWidget *XmlSourceEditorEditable::widget()
{
    return editorWidget;
}

bool XmlSourceEditorEditable::createNew(const QString &contents)
{
    return editorWidget->createNew(contents);
}

bool XmlSourceEditorEditable::open(const QString &fileName)
{
    return editorWidget->open(fileName);
}

int XmlSourceEditorEditable::currentLine() const
{
    if (editorWidget->currentEditorState() == XmlEditorWidget::SourceEditor)
        return TextEditor::PlainTextEditorEditable::currentLine();
    return 0;
}

int XmlSourceEditorEditable::currentColumn() const
{
    if (editorWidget->currentEditorState() == XmlEditorWidget::SourceEditor)
        return TextEditor::PlainTextEditorEditable::currentColumn();
    return 0;
}
