#ifndef XMLTREEPLUGIN_H
#define XMLTREEPLUGIN_H

#include "xmltreeplugin_global.h"

#include <extensionsystem/iplugin.h>

namespace XmlTreeEditor {
namespace Internal {

class XmlTreePluginImpl : public ExtensionSystem::IPlugin
{
    Q_OBJECT

public:
    XmlTreePluginImpl();
    ~XmlTreePluginImpl();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private slots:

private:

};

} // namespace Internal
} // namespace XmlTreePlugin

#endif // XMLTREEPLUGIN_H

