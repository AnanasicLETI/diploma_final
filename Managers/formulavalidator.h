#ifndef FORMULAVALIDATOR_H
#define FORMULAVALIDATOR_H

#include <QString>
#include <QMap>
#include <QPair>
#include <QDebug>

class FormulaValidator
{
public:
    FormulaValidator(QString newFormula, int newDimension);
    ~FormulaValidator();

    static bool validateFormulaStruct(QString formula, int dimension);

    const int TERM_NUMBER_MAX_LEN = 4;

private:

    bool isValid();

    QString formula;
    int formulaLen;
    int dimension;
    QMap<int, int> *primitive_term_map;
    QMap<int, int> *newPrimitive_term_map;

    bool isPrimitiveMapUpdated;

    bool isValidPrimitive_q_term();
    int getPrimitive_Exp_term();


    int  get_M_operationLength(QString formula, int pos);
    bool isPreviousOperation_M(QString formula, int pos, int *M_pos);

    bool checkMultiply_term(int pos, int len);
    bool checkUnion_term(int pos, int len);
    bool checkBrackets(int pos, int len);
    bool checkBracketOutsideExpansion(int *pos, int *len);
    bool checkTransform_term(int pos, int len);

    void addPrimitiveTermToNewMap(int pos, int len);
    void updateActivePrimitiveTermMap();


};

#endif // FORMULAVALIDATOR_H
