#include "xmltreeplugin.h"
#include "xmleditorfactory.h"

#include <QtPlugin>

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/mimedatabase.h>

#include <QtPlugin>
#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>


using namespace XmlTreeEditor::Internal;

XmlTreePluginImpl::XmlTreePluginImpl()
{
    // Create your members
}

XmlTreePluginImpl::~XmlTreePluginImpl()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool XmlTreePluginImpl::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // connect to other plugins' signals
    // "In the initialize method, a plugin can be sure that the plugins it
    //  depends on have initialized their members."

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)


    addAutoReleasedObject(new XmlEditorFactory(this));

    return true;
}

void XmlTreePluginImpl::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // "In the extensionsInitialized method, a plugin can be sure that all
    //  plugins that depend on it are completely initialized."
}

ExtensionSystem::IPlugin::ShutdownFlag XmlTreePluginImpl::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

Q_EXPORT_PLUGIN2(XmlTreePlugin, XmlTreePluginImpl)

