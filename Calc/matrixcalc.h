#ifndef MATRIXCALC_H
#define MATRIXCALC_H

#include <QObject>
#include <QLineEdit>
#include <QPair>

#include <QRegExp>

#include <QVector>
#include <qmath.h>

#include <QDebug>

#include "../Managers/graphtree.h"
#include "vectorRepresentation.h"
#include "mynumber.h"
#include "polandreversparser.h"

class MatrixCalc: public QObject
{
    Q_OBJECT

public:
    MatrixCalc();
    void changeInitialData(int Rank, int dimension);

    static unsigned int getDimension();
    static unsigned int getRank();

    static unsigned int getMatrixLen();
    static unsigned int getVectorLen();
    static unsigned int getVectorMaxElement();

    static void setDimension(unsigned int newDimension);
    static void setRank(unsigned int newRank);

    static bool convertDataToThirdCalc(QVector<bool> externalVector, QVector<QVector<bool> > addedVector, QString* str);
    static QVector<bool> decompose(QVector<bool> externalVector, QVector<bool> addedVector);
    static bool adding(QVector<bool> vector1, QVector<bool> vector2);

    static QString replaceAllVariablesInPolandParseByValues(QStringList *polandList,QMap<QString, MyNumber> *inputVectors);
    static QString replaceAllVariablesInPolandParseByValuesButSaveFormula(QList<QPair<QString, QString>> *polandList, QMap<QString, MyNumber> *inputVectors);
//    static QString getFormulaGraphTree(QStringList *polandList, int dimension);

    static MyNumber getSuperpose(MyNumber, QVector<MyNumber>, QString *log = nullptr);
    static QVector<MyNumber> getTranspose(MyNumber inputNum, QString *log = nullptr);
    static QVector<unsigned int> getIntersection(QString var_1, QString var_2, QString *log = nullptr);

    static QString getMatrixFormFromVectorInput(MyNumber input);
    static QString getVectorFormFromMatrixInput(MyNumber input);
    static QString calcFifth(QMap<QString, MyNumber> *inputVectors, QString formula);

private:
    static unsigned int rank;
    static unsigned int dimension;

};


#endif // MATRIXCALC_H
