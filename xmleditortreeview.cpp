#include "xmleditortreeview.h"
#include "xmltreemodel.h"
#include "xmltreenamedelegate.h"
#include "xmltreevaluedelegate.h"
#include <QHeaderView>
#include <QAction>
#include <QMenu>
#include <QDomNode>
#include <QContextMenuEvent>
#include <texteditor/texteditorconstants.h>
#include <QTextCharFormat>

#include <QDebug>

using namespace XmlTreeEditor;
using namespace XmlTreeEditor::Internal;


namespace XmlTreeEditor {
class XmlEditorTreeViewPrivate
{
public:
    QString content;
    bool modified;
    Internal::XmlTreeModel *treeModel;
    XmlEditorTreeView *q;
};
}

XmlEditorTreeView::XmlEditorTreeView(QWidget *parent) :
    QTreeView(parent), d(new XmlEditorTreeViewPrivate)
{
    d->q = this;
    d->treeModel = new XmlTreeModel(this);

    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->setDragEnabled(true);
    this->setDropIndicatorShown(true);
    this->setDragDropMode(QAbstractItemView::DragDrop);
    this->setAcceptDrops(true);

    this->setModel(d->treeModel);
    this->setItemDelegateForColumn(0, new XmlTreeNameDelegate);
    this->setItemDelegateForColumn(1, new XmlTreeValueDelegate);

    this->header()->resizeSection(0, 250);
    this->header()->resizeSection(1, 250);
    header()->setStretchLastSection(true);

    connect(d->treeModel, SIGNAL(xmlModified()), this, SLOT(onContentModified()));

    d->modified = false;

    addSubElementAction = new QAction(tr("Add Sub-element"), this);
    addAttributeAction = new QAction(tr("Add Attribute"), this);
    createRootElementAction = new QAction(tr("Create Root Element"), this);
    deleteNodeAction = new QAction(tr("Remove"), this);
    contextMenu = new QMenu(this);
    contextMenu->addAction(addSubElementAction);
    contextMenu->addAction(addAttributeAction);
    contextMenu->addAction(createRootElementAction);
    contextMenu->addAction(deleteNodeAction);



}

XmlEditorTreeView::~XmlEditorTreeView()
{
    delete d;
}


void XmlEditorTreeView::dropEvent ( QDropEvent *event )
{
    bool ctrlModifier = event->keyboardModifiers() & Qt::ControlModifier;
    bool shiftModifier = event->keyboardModifiers() & Qt::ShiftModifier;
    if (!ctrlModifier && !shiftModifier)
    {
        QAction *dropCopyAction = new QAction(tr("Copy Here")+"\tCtrl", this);
        dropCopyAction->setEnabled(true);
        QAction *dropMoveAction = new QAction(tr("Move Here")+"\tShift", this);
        QAction *dropSeparatorAction = new QAction(this);
        dropSeparatorAction->setSeparator(true);
        QAction *dropCancelAction = new QAction(tr("Cancel")+"\tEsc", this);
        QList<QAction *> actionList;
        actionList << dropCopyAction << dropMoveAction << dropSeparatorAction << dropCancelAction;
        QAction *selectedAction = QMenu::exec(actionList, QCursor::pos());
        ctrlModifier = selectedAction == dropCopyAction;
        shiftModifier = selectedAction == dropMoveAction;
        delete dropCopyAction;
        delete dropMoveAction;
        delete dropSeparatorAction;
        delete dropCancelAction;
    }
    if (shiftModifier)
        event->setDropAction(Qt::MoveAction);
    else if (ctrlModifier)
        event->setDropAction(Qt::CopyAction);
    if (ctrlModifier || shiftModifier)
        QAbstractItemView::dropEvent(event);
}



void XmlEditorTreeView::contextMenuEvent(QContextMenuEvent *ev)
{
    QModelIndex currIndex;
    if (this->selectionModel()->selectedIndexes().size() == 0)
    {
        if (model()->rowCount() == 0)
        {
            addSubElementAction->setVisible(false);
            addAttributeAction->setVisible(false);
            createRootElementAction->setVisible(true);
            deleteNodeAction->setVisible(false);
        }
        else
        {
            return;
        }
    }
    else
    {
        currIndex = this->selectionModel()->selectedIndexes().first();
        if (!currIndex.isValid())
        {
            if (model()->rowCount() == 0)
            {
                addSubElementAction->setVisible(false);
                addAttributeAction->setVisible(false);
                createRootElementAction->setVisible(true);
                deleteNodeAction->setVisible(false);
            }
            else
            {
                return;
            }
        }
        else
        {
            if (currIndex.data(XmlTreeModel::NodeTypeRole).toInt() == QDomNode::ElementNode)
            {
                addSubElementAction->setVisible(true);
                addAttributeAction->setVisible(true);
                createRootElementAction->setVisible(false);
                deleteNodeAction->setVisible(true);
            }
            else
            {
                addSubElementAction->setVisible(false);
                addAttributeAction->setVisible(false);
                createRootElementAction->setVisible(false);
                deleteNodeAction->setVisible(true);
            }
        }
    }
    QAction *selectedAction = contextMenu->exec(ev->globalPos());
    if (selectedAction == addSubElementAction)
    {
        d->treeModel->addNode(QDomNode::ElementNode, tr("Name"), "", currIndex);
    }
    else if (selectedAction == addAttributeAction)
    {
        d->treeModel->addNode(QDomNode::AttributeNode, tr("Name"), tr("Value"), currIndex);
    }
    else if (selectedAction == createRootElementAction)
    {
        d->treeModel->addNode(QDomNode::ElementNode, tr("Name"));
    }
    else if (selectedAction == deleteNodeAction)
    {
        d->treeModel->removeRows(currIndex.row(), 1, currIndex.parent());
    }
}

void XmlEditorTreeView::setFontSettings(const TextEditor::FontSettings &fs)
{
    const QTextCharFormat genericTextFormat =
            fs.toTextCharFormat(QLatin1String(TextEditor::Constants::C_TEXT));
    QTextCharFormat selectedFormat =
            fs.toTextCharFormat(QLatin1String(TextEditor::Constants::C_SELECTION));
    XmlTreeValueDelegate::valueFormat =
            fs.toTextCharFormat(QLatin1String(TextEditor::Constants::C_STRING));
    XmlTreeValueDelegate::commentFormat =
            fs.toTextCharFormat(QLatin1String(TextEditor::Constants::C_COMMENT));
    XmlTreeNameDelegate::attributeFormat =
            fs.toTextCharFormat(QLatin1String(TextEditor::Constants::C_FIELD));
    XmlTreeNameDelegate::elementFormat =
            fs.toTextCharFormat(QLatin1String(TextEditor::Constants::C_TYPE));
    XmlTreeNameDelegate::nodeTypeFormat =
            fs.toTextCharFormat(QLatin1String(TextEditor::Constants::C_KEYWORD));

    selectedFormat.clearBackground();

    XmlTreeNameDelegate::genericFormat = genericTextFormat;
    XmlTreeNameDelegate::selectedFormat = selectedFormat;
    XmlTreeValueDelegate::selectedFormat = selectedFormat;

    QPalette p = palette();
    p.setColor(QPalette::Text, genericTextFormat.foreground().color());
    p.setColor(QPalette::WindowText, genericTextFormat.foreground().color());
    p.setColor(QPalette::Base, genericTextFormat.background().color());
    p.setColor(QPalette::HighlightedText, selectedFormat.foreground().color());
    setPalette(p);


}

void XmlEditorTreeView::setContent(const QString &content)
{
    d->treeModel->populateFromXml(content);
    d->content = content;
    expandToDepth(0);
    d->modified = false;
}

QString XmlEditorTreeView::content()
{
    return d->modified ? d->treeModel->getXml() : d->content;
}

void XmlEditorTreeView::onContentModified()
{
    d->modified = true;
    emit contentModified();
}

