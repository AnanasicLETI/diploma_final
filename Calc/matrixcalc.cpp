#include "matrixcalc.h"

unsigned int MatrixCalc::rank = 0;
unsigned int MatrixCalc::dimension = 0;

MatrixCalc::MatrixCalc()
{

}


unsigned int MatrixCalc::getDimension()
{
    return dimension;
}

unsigned int MatrixCalc::getRank()
{
    return rank;
}

unsigned int MatrixCalc::getMatrixLen()
{
    return pow(MatrixCalc::getRank(), MatrixCalc::getDimension()+1);;
}

unsigned int MatrixCalc::getVectorLen()
{
    return pow(MatrixCalc::getRank(), MatrixCalc::getDimension());
}

unsigned int MatrixCalc::getVectorMaxElement()
{
    return pow(2, MatrixCalc::getRank()) - 1;;
}

void MatrixCalc::setDimension(unsigned int newDimension)
{
    dimension = newDimension;
}

void MatrixCalc::setRank(unsigned int newRank)
{
    rank = newRank;
}


//QVector<QVector<unsigned int> > MatrixCalc::parseInputFifth(QVector<QVector<unsigned int>> *inputVector, QString formula, int *transposeOrder)
//{

//    if(formula.indexOf("M") == -1){
//        *transposeOrder = 0;
//    }else{
//        QString transNumber = formula.mid(formula.indexOf("M")+1, 1);
//        *transposeOrder = transNumber.toInt();
//    }
//    QString mainSuperposeVectorNum = formula.mid(formula.indexOf("(q")+2, 1);


//    QString tmp = formula.mid(formula.indexOf("*q")+1, (formula.indexOf(")") - formula.indexOf("*q")-1) );
//    QStringList vectorNumList = tmp.split(",");

//    QVector<QVector<unsigned int>> newOut;
//    QVector<unsigned int> a = inputVector->at(mainSuperposeVectorNum.toInt());
//    newOut.append( QVector<unsigned int>(inputVector->at( mainSuperposeVectorNum.toInt()-1 )) );

//    for(int i=0; i < inputVector->size()-1; i++){
//        QString num = vectorNumList[i].mid(1);
//        newOut.append( QVector<unsigned int>( inputVector->at( num.toInt()-1 ) ) );
//    }
//    return newOut;
//}

bool MatrixCalc::convertDataToThirdCalc(QVector<bool> externalVector, QVector<QVector<bool>> addedVector, QString* str)
{
    for(int i=0; i < addedVector.size();i++){
        *str += VectorRepresentation::getStrFromVectorInt(externalVector);
        for(int j=i;j < addedVector.size();j++){
            *str += " x ";
            *str += VectorRepresentation::getStrFromVectorInt(addedVector[j]);
        }
        *str += " = ";
        externalVector=decompose(externalVector, addedVector[i]);
    }
    *str += QString::number(externalVector[0]);
    return externalVector[0];
}

QVector<bool> MatrixCalc::decompose(QVector<bool> externalVector, QVector<bool> addedVector)
{
    int size = addedVector.size();
    int k = externalVector.size()/size;
    QVector<bool> outVector;
    for(int i=0; i<k; i++){
        QVector<bool> tmp = externalVector.mid(i*size, size);
        outVector.append(adding(addedVector, tmp));
    }
    return outVector;
}

bool MatrixCalc::adding(QVector<bool> vector1, QVector<bool> vector2)
{
    int size = vector1.size();
    bool out = 0;
    for(int i=0; i < size; i++){
        out = out | (vector1[i]*vector2[i]);
    }
    return out;
}


QString MatrixCalc::replaceAllVariablesInPolandParseByValues(QStringList *polandList, QMap<QString, MyNumber> *inputVectors)
{
    QString outStr;
    for(int i=0; i < polandList->size();i++){
        QString tmp = polandList->at(i);
        if( tmp[0] == "q"){
            QString formula = polandList->at(i);
            MyNumber num = inputVectors->value(formula);
            QString value = num.getStringRepresentation(MyNumber::TYPE_MATRIX);
            polandList->replace( i, value);
        }
    }
    qDebug() << *polandList;
    return outStr;
}

QString MatrixCalc::replaceAllVariablesInPolandParseByValuesButSaveFormula(QList<QPair<QString, QString> > *polandList, QMap<QString, MyNumber> *inputVectors)
{
    QString outStr;
    for(int i=0; i < polandList->size();i++){
        QString tmp = polandList->at(i).first;
        if( tmp[0] == "q"){
            QString formula = polandList->at(i).first;
            MyNumber num = inputVectors->value(formula);
            QString value = num.getStringRepresentation(MyNumber::TYPE_MATRIX);
//            QString value = getStrFromVectorInt( inputVectors->at( QString(tmp[1]).toInt() -1 ) );
            polandList->replace( i, QPair<QString, QString>(value, tmp));
        }
    }
    qDebug() << *polandList;
    return outStr;
}

MyNumber MatrixCalc::getSuperpose(MyNumber inputMain, QVector<MyNumber> inputVectors, QString *log)
{
    QString outStr;
    QVector<unsigned int> output;
    QVector< QVector<bool> > externalMatrix;
    foreach(MyNumber num, inputVectors){
        externalMatrix.append( num.getMatrixRepresentationByLongVector() );
    }
    int len = getMatrixLen();
    int localLen = len/rank;

    outStr += inputMain.getStringRepresentation(MyNumber::TYPE_MATRIX)+"*";
    for(int i = 0; i < inputVectors.size(); i++){
        outStr += inputVectors[i].getStringRepresentation(MyNumber::TYPE_MATRIX);
        if( i != inputVectors.size()-1){
           outStr += ",";
        }
    }
    outStr += ":\n";

    for(unsigned int i = 0; i < rank; i++){
        QVector<bool> externalVector = inputMain.getMatrixRepresentationByLongVector().mid((i)*localLen, localLen);
        for(int j=0; j < localLen;j++){
            QVector<QVector<bool>> add;

            for(int n = MatrixCalc::getDimension(); n > 0; n--){
                add.append(QVector<bool>());

                for(unsigned int m=0; m< rank;m++){
                    add.last().append(externalMatrix[n-1][m*localLen+j]);
                }
            }
            QString tmpStr = "";
            output.append(convertDataToThirdCalc(externalVector, add, &tmpStr));
            outStr += tmpStr + "\n";
        }
        outStr += "--------------------\n";
    }
    MyNumber number = MyNumber(VectorRepresentation::getStrFromVectorInt(output), MyNumber::TYPE_MATRIX, getRank(), getDimension());
    outStr += "S: "+ number.getExtendetStringRepresentation(MyNumber::TYPE_MATRIX, QString("S: ").length() );
    if(log != nullptr ){
        *log += outStr + "\n";
    }
    return number;
}

QVector<MyNumber> MatrixCalc::getTranspose(MyNumber inputNum, QString *log)
{
    QVector<MyNumber> tranposed = inputNum.operator_allM();
    for(int i=0; i < tranposed.size(); i++){
        if(log != nullptr){
//            *log += inputNum.getStringRepresentation(MyNumber::TYPE_MATRIX) +":\n";
            QString termName = "T"+QString::number(i+1)+"= ";
            *log += termName;
            *log += tranposed[i].getExtendetStringRepresentation(MyNumber::TYPE_MATRIX,
                                                                 termName.length()) +"\n\n";
        }
    }
    return tranposed;
}

QVector<unsigned int> MatrixCalc::getIntersection(QString var_1, QString var_2, QString *log)
{

    QString v1, v2, v3;
    QString outStr = "";
    v1 = var_1;
    v2 = var_2;
    QVector<unsigned int> vec_1 = VectorRepresentation::getVectorIntFromStr( var_1 );
    QVector<unsigned int> vec_2 = VectorRepresentation::getVectorIntFromStr( var_2 );
    QVector<unsigned int> outVector = vec_1;

    for(int i=0; i < vec_1.size(); i++){
        outVector[i] &= vec_2[i];  // This is matrixForm and & is safe
    }
    v3 = VectorRepresentation::getStrFromVectorInt(outVector);

    outStr += v1+" ⋂\n"+v2+"\n----------\n"+v3;
    if(log != nullptr){
         *log += outStr;
    }

    return outVector;
}

QString MatrixCalc::getMatrixFormFromVectorInput(MyNumber input)
{
    QString outStr;
    for(unsigned int i=0; i < MatrixCalc::getRank(); i++){
        int localRank = pow(2, i);
        outStr += "{"+ QString::number( localRank ) +"}: ";
        outStr += VectorRepresentation::getStrFromVectorInt( input.getPrimaryDecomposition( i ) ) + "\n";
    }
    outStr += "Итог:" + input.getExtendetStringRepresentation( MyNumber::TYPE_MATRIX, QString("Итог:").length() );
    return outStr;
}

QString MatrixCalc::getVectorFormFromMatrixInput(MyNumber input)
{
    QString outStr;

    QString result = input.getExtendetStringRepresentation(MyNumber::TYPE_VECTOR, 0);
    for(unsigned int i=0; i < MatrixCalc::getRank(); i++){
        int localRank = pow(2, i);
        outStr += "{"+ QString::number( localRank ) +"}: ";
        QVector<bool> isPrimeContained = input.getPrimaryDecomposition( i );

        for(int j=0; j < isPrimeContained.size(); j++){
            int prime = isPrimeContained[j] * localRank;
            outStr += QString::number( prime ) ;
        }
        outStr += "\n";
    }
    outStr += "Итог:" + result;

    return outStr;
}



QString MatrixCalc::calcFifth( QMap<QString, MyNumber> *inputVectors, QString formula)
{
    QString outStr;

    QStringList calcList = PolandParser::getPolandParse(formula);
    qDebug() << "Old Calc" << calcList;
    QList<QPair<QString, QString>> newCalcList;
    for(int i=0; i < calcList.size(); i++) {
        newCalcList.append( QPair<QString, QString>(calcList[i], "") );
    }
    qDebug() << "New Calc" << newCalcList;

    outStr += replaceAllVariablesInPolandParseByValues(&calcList, inputVectors);
    outStr += replaceAllVariablesInPolandParseByValuesButSaveFormula(&newCalcList, inputVectors);

    QStringList valueStack;
    QList<QPair<QString, QString>> doubleValueStack;
    while(!newCalcList.isEmpty()){
        QPair<QString, QString> activePair = newCalcList.takeFirst();
        QString tmp = activePair.first;
        qDebug() << tmp << calcList << newCalcList << valueStack << outStr;
        if(tmp == "⋂"){
            qDebug() << "opration⋂" << activePair << doubleValueStack;
            QPair<QString, QString> term_2 = doubleValueStack.takeLast();
            QPair<QString, QString> term_1 = doubleValueStack.takeLast();

            QString var_2 = term_2.first;
            QString var_1 = term_1.first;
            QString newFormula = "("+ term_1.second +")⋂("+ term_2.second +")";
            outStr += "Вычисления в вершине: "+newFormula+"\n";
            QString var_3 = VectorRepresentation::getStrFromVectorInt( getIntersection(var_1, var_2) );
            QPair<QString, QString> newValue = QPair<QString, QString>(var_3, newFormula);
            outStr += var_1 +" ⋂ "+ var_2 +" = "+ var_3 +"\n";
            doubleValueStack.append(newValue);
            valueStack.append( var_3 );

            outStr += "\n";
            qDebug() << "opration⋂_end";
            continue;
        }

        if(tmp == "*"){
            qDebug() << "opration*" << activePair;
            QVector<MyNumber> inputVectors;
            QVector<QString> formulaVec;
            for (unsigned int i=0; i < getDimension(); i++){
                qDebug() << "1" << doubleValueStack;
                QPair<QString, QString> tmpPair = doubleValueStack.takeLast();
                inputVectors.push_front( MyNumber(tmpPair.first, MyNumber::TYPE_MATRIX,
                                                  getRank(), getDimension()) );
                formulaVec.push_front(tmpPair.second);
                qDebug() << "2" << inputVectors.size();
            }
            QPair<QString, QString> tmpPair = doubleValueStack.takeLast();
            MyNumber inputMain = MyNumber( tmpPair.first, MyNumber::TYPE_MATRIX,
                                           getRank(), getDimension());
            QString mainFormula = tmpPair.second;
            QString newFormula = "(" + mainFormula +" * ";
            for(int i=0; i<formulaVec.size(); i++) {
                newFormula += formulaVec[i];
                if(i != formulaVec.size()-1){
                    newFormula += ",";
                }
            }newFormula += ")";
            outStr += "Вычисления в вершине: "+newFormula+"\n";
            QString result = getSuperpose(inputMain, inputVectors, &outStr).getStringRepresentation(MyNumber::TYPE_MATRIX);
            doubleValueStack.append( QPair<QString, QString>( result, newFormula) );
            qDebug() << "after *" << doubleValueStack;
            valueStack.append( result );

            outStr += "\n";
            qDebug() << "opration*_end";
            continue;
        }
        if(tmp[0] == 'M'){
            qDebug() << "oprationM" << activePair;
            QPair<QString, QString> tmpPair = doubleValueStack.takeLast();
            MyNumber inputMain = MyNumber( tmpPair.first, MyNumber::TYPE_MATRIX,
                                             getRank(), getDimension());
            int numOfTranpose = QString(tmp[1]).toInt();
            QString newFormula = "M"+ QString::number(numOfTranpose) + tmpPair.second;
            outStr += "Вычисления в вершине: "+newFormula+"\n";

            QVector<MyNumber> transposed = getTranspose(inputMain, &outStr);
            QString result = transposed[numOfTranpose-1].getStringRepresentation(MyNumber::TYPE_MATRIX) ; //(numOfTranpose-1)
            doubleValueStack.append( QPair<QString, QString>( result, newFormula) );
            valueStack.append( result );

            outStr += "\n";
            qDebug() << "oprationM_end";
            continue;
        }
        doubleValueStack.append(activePair);
        valueStack.append( tmp );
    }

    QString result =  doubleValueStack.first().first;// valueStack.first();
    qDebug() << "Formula lalalalalalal" << result;
    MyNumber num = MyNumber(result, MyNumber::TYPE_MATRIX, getRank(), getDimension());

    QString vecResult = num.getExtendetStringRepresentation(MyNumber::TYPE_VECTOR, 0);
    QString matrixResult = num.getExtendetStringRepresentation(MyNumber::TYPE_MATRIX, QString("t= ").length() );

qDebug() << "Formula lalalalalalal";
    outStr += "\nРезультат в векторной форме:\nt= "+vecResult+"\nРезультат в матричной форме:\nt= "+matrixResult+"\n";
qDebug() << "Result" << valueStack;

    return outStr;
}

//QVector<unsigned int> MatrixCalc::calcFifthSection(QVector<QVector<unsigned int> > *inputVectors, QString formula, QString *log)
//{
//    int transposeNumber;

//    QVector<QVector<unsigned int>> parsedVector = parseInputFifth(inputVectors, formula, &transposeNumber);
//    QVector<unsigned int> mainVector = parsedVector.takeFirst();
//    QVector<QVector<unsigned int>> vectors = parsedVector;

//    QString outStr;
//    qDebug() << "calcFifth" << mainVector << vectors;
//    QVector<unsigned int> result;
//    //    result = getSuperpose(mainVector, vectors, &outStr);
//    outStr += "\n";

//    if(transposeNumber != 0){
//    //        QVector<QVector<unsigned int>> tranposeResult = getTranspose(result);
//    //        result = tranposeResult[transposeNumber-1];
//    //        outStr += "T: " + getStrFromVectorInt(result);
//    //        outStr += "\n";
//    }
//    if(log != nullptr){
//        *log += outStr;
//    }
//    return result;
//}

