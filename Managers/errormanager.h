#ifndef ERRORMANAGER_H
#define ERRORMANAGER_H

#include <QObject>
#include "validationmanager.h"

class ErrorManager : public QObject
{
    Q_OBJECT
public:
    ErrorManager();

    static QString getErrString_ByErrCode(int errCode);
    static QString getExtendedErrString_ByErrCode(int errCode);

};

#endif // ERRORMANAGER_H
