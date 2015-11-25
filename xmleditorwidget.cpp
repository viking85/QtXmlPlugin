#include "xmleditorwidget.h"
#include "xmleditortreeview.h"
#include "xmlsourceeditor.h"
#include "xmleditorconstants.h"
#include <coreplugin/ifile.h>
#include <texteditor/basetextdocument.h>
#include <QLabel>

#include <QDebug>

using namespace XmlTreeEditor;

namespace XmlTreeEditor {
class XmlEditorWidgetPrivate
{
public:
    XmlEditorTreeView *treeView;
    XmlSourceEditor *sourceEditor;
    XmlEditorWidget *q;
    bool contentModifiedFromLastTabSwitch;
};
}

XmlEditorWidget::XmlEditorWidget(QWidget *parent) :
    QTabWidget(parent), d(new XmlEditorWidgetPrivate)
{
    d->q = this;
    d->treeView = new XmlEditorTreeView(this);
    this->addTab(d->treeView, tr("Tree"));
    d->sourceEditor = new XmlSourceEditor(this);
    d->sourceEditor->setMimeType(Constants::XML_MIME_TYPE);
    d->contentModifiedFromLastTabSwitch = false;
    this->addTab(d->sourceEditor, tr("Source"));
    this->setTabShape(QTabWidget::Triangular);
    this->setTabPosition(QTabWidget::South);
    this->setDocumentMode(true);
    d->treeView->setFrameStyle(QFrame::NoFrame);

    connect(this, SIGNAL(currentChanged(int)), d->sourceEditor, SIGNAL(cursorPositionChanged()));
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(slotCurrentChanged(int)));
    connect(d->sourceEditor, SIGNAL(textChanged()), this, SLOT(slotSourceContentModified()));
    connect(d->treeView, SIGNAL(contentModified()), this, SLOT(slotTreeContentModified()));

    setToolbarLineColumnLabelVisible(false);
}

XmlEditorWidget::~XmlEditorWidget()
{
    disconnect(SIGNAL(currentChanged(int)));
    this->clear();
    //Removing sourceEditor from hierarchy for proper deleting it from Editable
    d->sourceEditor->setParent(0);
    delete d;
}

Core::IFile *XmlEditorWidget::file()
{
    return d->sourceEditor->file();
}

XmlEditorTreeView *XmlEditorWidget::treeView()
{
    return d->treeView;
}

XmlSourceEditor *XmlEditorWidget::sourceEditor()
{
    return d->sourceEditor;
}

bool XmlEditorWidget::createNew(const QString &contents)
{
    bool result = d->sourceEditor->createNew(contents);
    if (result)
        d->treeView->setContent(d->sourceEditor->document()->toPlainText());
    d->contentModifiedFromLastTabSwitch = false;
    d->sourceEditor->baseTextDocument()->document()->setModified(false);
    return result;
}

bool XmlEditorWidget::open(const QString &fileName)
{
    bool result = d->sourceEditor->open(fileName);
    if (result)
        d->treeView->setContent(d->sourceEditor->document()->toPlainText());
    d->contentModifiedFromLastTabSwitch = false;
    d->sourceEditor->baseTextDocument()->document()->setModified(false);
    return result;
}

void XmlEditorWidget::documentSaved()
{
    d->contentModifiedFromLastTabSwitch = false;
}

QByteArray XmlEditorWidget::saveState() const
{
    return d->sourceEditor->saveState();
}

bool XmlEditorWidget::restoreState(const QByteArray &state)
{
    return d->sourceEditor->restoreState(state);
}

QString XmlEditorWidget::displayName() const
{
    return d->sourceEditor->displayName();
}

void XmlEditorWidget::setDisplayName(const QString &title)
{
    d->sourceEditor->setDisplayName(title);
}

void XmlEditorWidget::updateTab(EditorState tab)
{
    switch (tab)
    {
    case SourceEditor:
        d->sourceEditor->setPlainText(d->treeView->content());
        d->sourceEditor->baseTextDocument()->document()->setModified(true);
        break;
    case TreeViewEditor:
        d->treeView->setContent(d->sourceEditor->document()->toPlainText());
    default:
        break;
    }
}

void XmlEditorWidget::slotCurrentChanged(int index)
{
    switch (index)
    {
    case SourceEditor:
        setToolbarLineColumnLabelVisible(true);
        break;
    case TreeViewEditor:
        setToolbarLineColumnLabelVisible(false);
        break;
    default:
        break;
    }
    if (d->contentModifiedFromLastTabSwitch)
    {
        updateTab(index == SourceEditor ? SourceEditor : TreeViewEditor);
    }
    d->contentModifiedFromLastTabSwitch = false;
}

void XmlEditorWidget::slotTreeContentModified()
{
    d->contentModifiedFromLastTabSwitch = true;
    d->sourceEditor->baseTextDocument()->document()->setModified(true);
}

void XmlEditorWidget::slotSourceContentModified()
{
    if (d->sourceEditor->baseTextDocument()->document()->isModified())
        d->contentModifiedFromLastTabSwitch = true;
}

XmlEditorWidget::EditorState XmlEditorWidget::currentEditorState()
{
    if (currentWidget() == d->sourceEditor)
        return SourceEditor;
    return TreeViewEditor;
}

void XmlEditorWidget::setToolbarLineColumnLabelVisible(bool visible)
{
    if (!d->sourceEditor->editableInterface()->toolBar())
        return;
    QList<QLabel *> labels = d->sourceEditor->editableInterface()->toolBar()->findChildren<QLabel *>();
    if (!labels.size())
        return;
    labels[0]->setVisible(visible);
}
