#include "formulavalidator.h"

FormulaValidator::FormulaValidator(QString newFormula, int newDimension)
{
    formula = newFormula;
    formulaLen = newFormula.length();
    dimension = newDimension;
    primitive_term_map = new QMap<int, int>;
}

FormulaValidator::~FormulaValidator()
{
    delete primitive_term_map;
}

bool FormulaValidator::validateFormulaStruct(QString formula, int dimension)
{
    if( formula.isEmpty() ){
        return false;
    }
    FormulaValidator tmpValidator = FormulaValidator(formula, dimension);
    bool validFlag = tmpValidator.isValid();
    return validFlag;
}

bool FormulaValidator::isValid()
{

    if( !isValidPrimitive_q_term() ){
        return false;
    }
    while( getPrimitive_Exp_term() ){}

    bool flag;
    if( !primitive_term_map->isEmpty() &&
        primitive_term_map->first() == formulaLen ){
        flag = true;
    }else{
        flag = false;
    }

    qDebug() << "+++++++++++++++++++++++++++++++++++++++++\n" <<
                *(primitive_term_map) << flag << "\n" <<
                "+++++++++++++++++++++++++++++++++++++++++\n";
    return flag;
}

bool FormulaValidator::isValidPrimitive_q_term()
{
    for(int i=0; i<formulaLen; i++){
       if(formula.at(i) == 'q'){
           int pos = i++;
           int len = 0;
           while(i<formulaLen && formula.at(i).isDigit() ){
               len++;i++;
           }
           if(len == 0 || len > TERM_NUMBER_MAX_LEN || (i>formulaLen)){
               qDebug() << "isValidPrimitive_q_term FALSE";
               return false;
               // TODO error signal;
           }else{
               primitive_term_map->insert(pos, len+1);
           }
       }
    }
    return true;
}

int FormulaValidator::getPrimitive_Exp_term()
{
    newPrimitive_term_map = new QMap<int, int>;

    isPrimitiveMapUpdated = false;

    for(QMap<int, int>::iterator i=primitive_term_map->begin();
        i != primitive_term_map->end();
        i++){

        int pos = i.key();
        int len = i.value();
        qDebug() << "p/l" << pos << len;



        if(checkTransform_term(pos, len)){
            continue;
        }

        if(formula.length() == pos+len){
            newPrimitive_term_map->insert(i.key(), i.value());
            continue;
        }

        if( checkUnion_term(pos, len) ){
            i++;
            continue;
        }

        if( checkMultiply_term(pos, len) ){
            i+=dimension;
            continue;
        }

        if( checkBrackets(pos, len) ){
            continue;
        }

        newPrimitive_term_map->insert(pos, len);
    }

    updateActivePrimitiveTermMap();

    qDebug() << "isPrimitiveMapUpdated" << isPrimitiveMapUpdated;

    if(isPrimitiveMapUpdated){
        return true;
    }else{
        return false;
    }
}

bool FormulaValidator::isPreviousOperation_M(QString formula, int pos, int *M_pos)
{

    int newPos = pos-1;
//    if( (newPos>0)
//            && (formula.at(newPos) == "(") ){
//        newPos -= 1;
//    }else{
//        return false;
//    }
    while( (newPos>0) && formula.at(newPos).isDigit() ){
        newPos--;
    }
    qDebug() << "isPreviousOperation_M" << (formula.at(newPos)) << (pos - newPos) << (pos - newPos - 1) << newPos;
    if((formula.at(newPos) != 'M') ||
            (pos - newPos - 1) <= 0 ||
            (pos - newPos - 1) > TERM_NUMBER_MAX_LEN){
        return false;
    }
    *M_pos = newPos;
    return true;
}

bool FormulaValidator::checkMultiply_term(int pos, int len)
{
    if( formula.mid(pos+len, 1) == "*" ){
        int posOffset = pos+len+1;
        qDebug() << "* operation";

        for(int j=0; j<dimension; j++){
            qDebug() << "Len " << posOffset;
            if( primitive_term_map->contains(posOffset) ){
                posOffset+= primitive_term_map->value(posOffset);
                qDebug() << "addedLen formulaLen " << posOffset;
                if( j != dimension-1 ) {
                    if((posOffset+1 < formulaLen) && (formula.mid(posOffset, 1) == ",") ){
                        posOffset += 1;
                    }else{
                        return false;
                    }
                }
            }else{
                return false;
            }
        }

        len = posOffset-pos;
        addPrimitiveTermToNewMap(pos, len);

        return true;
    }else{
        return false;
    }
}

bool FormulaValidator::checkUnion_term(int pos, int len)
{
    if(formula.mid(pos+len, 1) == "⋂"){
        if( primitive_term_map->contains(pos+len+1) ){
            int newLen = len+primitive_term_map->value(pos+len+1)+1;
            addPrimitiveTermToNewMap(pos, newLen);
            return true;
        }
    }
    return false;
}

bool FormulaValidator::checkBrackets(int pos, int len)
{
    if( formula.mid(pos+len, 1) == ")" ){
//        if( checkTransform_term(pos, len) ){
//            return true;
//        }
        if( checkBracketOutsideExpansion(&pos, &len) ){
            qDebug() << "Bracket hugs";
            addPrimitiveTermToNewMap(pos, len);
            return true;
        }
    }
    return false;
}

bool FormulaValidator::checkBracketOutsideExpansion(int *pos, int *len)
{
    if( (*pos-1 < 0) || ( (*pos+*len) >= formulaLen) ){
        return false;
    }
    if( formula.at(*pos-1) == '(' && formula.at(*pos+*len) == ')' ){
        *len += 2;
        *pos -= 1;
        return true;
    }
    return false;
}

bool FormulaValidator::checkTransform_term(int pos, int len)
{
    int M_pos;
    if( isPreviousOperation_M(formula, pos, &M_pos) ){
        qDebug() << "M operation";
        int newLen = pos-M_pos+len;// +1 if with bracket ')'
        qDebug() << M_pos << newLen;
        addPrimitiveTermToNewMap(M_pos, newLen);
        return true;
    }
    return false;
}

void FormulaValidator::addPrimitiveTermToNewMap(int pos, int len)
{
    newPrimitive_term_map->insert(pos, len);
    isPrimitiveMapUpdated = true;
}

void FormulaValidator::updateActivePrimitiveTermMap()
{
        qDebug() <<*newPrimitive_term_map << *primitive_term_map;
        delete primitive_term_map;

        // TODO переделать (менять ссылку на объект)
        primitive_term_map = new QMap<int, int>(*newPrimitive_term_map);
        delete newPrimitive_term_map;
}
