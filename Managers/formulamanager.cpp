#include "formulamanager.h"

FormulaManager::FormulaManager()
{

}

bool FormulaManager::isValidFormulaStruct(QString formula, int dimension)
{
    return FormulaValidator::validateFormulaStruct(formula, dimension);
}

QStringList FormulaManager::get_q_WithIndex(QString formula)
{
    qDebug() << "get_q_WithIndex" << formula << formula.isEmpty();
    QStringList outStrList = QStringList();
    if( formula.isEmpty() ){
        return outStrList;
    }
    for(QString::Iterator i=formula.begin();
        i != formula.end();
        i++){
        QString number="";
        if( *i == "q" ){
            i++;
            while( i!=formula.end() && i->isDigit() ){
                number += *i;
                i++;
            }
            if( !number.isEmpty() ){
                outStrList.append("q"+number);
            }
            if( i == formula.end() ){
                break;
            }
        }
    }
    outStrList.removeDuplicates();
    return outStrList;
}

int FormulaManager::getMax_M_index(QString formula)
{

    int maxValue = 0;
    for(QString::Iterator i = formula.begin();
            i != formula.end();
            i++){

        if(i->unicode() == 'M' ){
            qDebug() << "getMax_M_index";
            QString number = "";i++;
            while( (i != formula.end()) && (i->isDigit()) ){
               number += *i;
               i++;
            }
            qDebug() << number;
            int newValue = number.toInt();
            if(newValue > maxValue){
                maxValue = newValue;
            }
        }
    }
    qDebug() << maxValue;
    return maxValue;

}
