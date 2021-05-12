#ifndef FORMULAMANAGER_H
#define FORMULAMANAGER_H

#include <QDebug>
#include <QString>
#include <QStringList>
#include "formulavalidator.h"


class FormulaManager
{
public:
    FormulaManager();

    static bool isValidFormulaStruct(QString formula, int dimension);
    static QStringList get_q_WithIndex(QString formula);
    static int  getMax_M_index(QString formula);
};

#endif // FORMULAMANAGER_H
