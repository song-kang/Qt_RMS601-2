#ifndef PROTOCOL_GW103_GLOBAL_H
#define PROTOCOL_GW103_GLOBAL_H

#include <QtCore>
#include <QtCore/qglobal.h>
#include <QtGui>

#ifdef PROTOCOL_GW103_LIB
# define PROTOCOL_GW103_EXPORT Q_DECL_EXPORT
#else
# define PROTOCOL_GW103_EXPORT Q_DECL_IMPORT
#endif

#endif // PROTOCOL_GW103_GLOBAL_H
