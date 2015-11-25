#ifndef XMLEDITORFACTORY_H
#define XMLEDITORFACTORY_H
#include <coreplugin/editormanager/ieditorfactory.h>

namespace XmlTreeEditor {
namespace Internal {
class XmlTreePluginImpl;

class XmlEditorFactoryPrivate;

class XmlEditorFactory : public Core::IEditorFactory
{
    Q_OBJECT
public:
    explicit XmlEditorFactory(XmlTreeEditor::Internal::XmlTreePluginImpl *owner);
    ~XmlEditorFactory();

    QStringList mimeTypes() const;

    QString id() const;
    QString displayName() const;

    Core::IFile *open(const QString &fileName);
    Core::IEditor *createEditor(QWidget *parent);


signals:

public slots:


protected:
    XmlEditorFactoryPrivate *d;

private:
    friend class XmlEditorFactoryPrivate;

};

}
}

#endif // XMLEDITORFACTORY_H
