#ifndef VALIDATIONMANAGER_H
#define VALIDATIONMANAGER_H

#include <QString>
#include <QStringList>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QVector>
#include <QMap>
#include <QPair>
#include <QDebug>
#include <QtMath>

//#include "formulavalidator.h"
#include "formulamanager.h"
#include "errormanager.h"
#include "Calc/vectorRepresentation.h"
#include "Calc/matrixcalc.h"
#include "../TermList/qtermlistmanager.h"

class ValidationManager
{
public:
    ValidationManager();
    static const int q_TERM_MAX_LEN = 4;
//    static QStringList getUnic_q_StringList(QString formula);


//    static bool isValidVector(QString Vector);
//    static bool isValidMatrix(QString Matrix);

    static int  vectorValidateWithErrCode(QString Vector);
    static int  matrixValidateWithErrCode(QString Matrix);

    static bool isDigitalString(QString str);

    const static int ERR_WRONG_SEQUENCE_LEN         = 0b00001;
    const static int ERR_WRONG_SYMBOL_IN_SEQUENCE   = 0b00010;
    const static int ERR_SEQUENCE_OUT_FROM_RANGE    = 0b00100;

    const static int ERR_WRONG_AMOUNT_OF_SEQUENCE   = 0b01000;
    const static int ERR_WRONG_FORMULA              = 0b10000;


    static int validateFirst(QPlainTextEdit *lineEdit, QString *errorMessage);
    static int validateSecond(QPlainTextEdit *lineEdit, QString *errorMessage);
    static int validateThird(QLineEdit *lineEdit, qTermListManager *termList, QString *errorMessage);
    static int validateFouth(QPlainTextEdit *lineEdit, QString *errorMessage);
    static int validateFifth(QLineEdit *lineEdit, qTermListManager *termList, QString *errorMessage, unsigned int type);

    static unsigned int validateAllLineEditForFormula(qTermListManager *termList, unsigned short type);
    static unsigned int validateAllLineEditForSuperpose(qTermListManager *termList, unsigned short type);
};

#endif // VALIDATIONMANAGER_H
