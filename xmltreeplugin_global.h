#ifndef XMLTREEPLUGIN_GLOBAL_H
#define XMLTREEPLUGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(XMLTREEPLUGIN_LIBRARY)
#  define XMLTREEPLUGINSHARED_EXPORT Q_DECL_EXPORT
#else
#  define XMLTREEPLUGINSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // XMLTREEPLUGIN_GLOBAL_H
