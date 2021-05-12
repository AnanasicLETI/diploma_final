#ifndef MYNUMBER_H
#define MYNUMBER_H

#include <QVector>
#include <QtMath>
#include <QDebug>
#include "vectorRepresentation.h"

class MyNumber
{
public:
    MyNumber();
    MyNumber( QVector<unsigned int> Vector, int newRank, int newDimension);
    MyNumber( QVector< QVector<bool> > Matrix, int newRank, int newDimension);

    MyNumber( QString input, int type, int newRank, int newDimension);

    QVector<unsigned int> getVectorRepresentation();
    QVector< QVector<bool> > getMatrixRepresentation();
    QVector<bool> getMatrixRepresentationByLongVector();

    // S - superpose,   *
    // M - tranpose,    M
    // Crossing         ⋂

    MyNumber operator_S(QVector<MyNumber>);
    MyNumber operator_M(int index);
    QVector<MyNumber> operator_allM();
    MyNumber operator_C(MyNumber);

    QVector< bool > getPrimaryDecomposition(int matrixRank);
    static QVector<unsigned int> getVectorFromMatrix( QVector< QVector<bool> > input);
    static QVector< QVector<bool> >getMatrixFromVector( QVector<unsigned int> input,
                                                        int rank);
    static int getStringLen(QString str, int type);

    static const int TYPE_MATRIX = 1;
    static const int TYPE_VECTOR = 2;

    QString getStringRepresentation(const int type);
    QString getExtendetStringRepresentation(const int type, int offset);

private:
    int rank;
    int dimension;

    QVector< QVector<bool> > number;

};

#endif // MYNUMBER_H
