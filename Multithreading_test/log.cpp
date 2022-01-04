#include "log.h"

#include <QThread>

QString threadIdString()
{
    return QString::number(reinterpret_cast<qint64>(QThread::currentThreadId())).rightJustified(5, '0');
}
