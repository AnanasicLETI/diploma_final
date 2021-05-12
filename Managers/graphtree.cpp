#include "graphtree.h"

GraphTree::GraphTree(QString formula)
{

    QString M_String = "";
    if(formula.indexOf("M") != -1){
        M_String = formula.mid(formula.indexOf("M"), 2);
    }

    QString q_String = formula.mid(formula.indexOf("(q")+1, 2);
    QString tmp = formula.mid(formula.indexOf("*q")+1, (formula.indexOf(")") - formula.indexOf("*q")-1) );
    QStringList vectorNumList = tmp.split(",");

    for(int i=0; i < vectorNumList.size(); i++){
        q_String += " "+vectorNumList[i];
    }

    int left = q_String.size()/2;

    if(M_String != ""){
        representation.append(QString(left, ' ')+M_String);
    }
    representation.append(QString(left, ' ')+"*");
    representation.append(q_String);
}

GraphTree::GraphTree(QStringList newRepresentation)
{
    representation = newRepresentation;
}

QString GraphTree::getFormulaGraphTree(QStringList *polandList, int dimension)
{
    QVector<GraphTree> valueStack;

    while(!polandList->isEmpty()){
        QString tmp = polandList->takeFirst();
        if(tmp == "⋂"){
            GraphTree var_2 = valueStack.takeLast();
            GraphTree var_1 = valueStack.takeLast();
            GraphTree var_3 = var_1 + var_2;
            valueStack.append( var_3 );
            continue;
        }
        if(tmp == "*"){
            QVector<GraphTree> inputVectors;
            for (int i=0; i < dimension; i++){
                inputVectors.append( valueStack.takeLast() );
            }
            GraphTree inputMain = valueStack.takeLast() ;
            GraphTree result = inputMain.operation_Superpose(inputVectors);     //getStrFromVectorInt(getSuperpose(inputMain, inputVectors, &outStr));
            valueStack.append( result );
            continue;
        }
        if(tmp[0] == 'M'){
            GraphTree inputMain = valueStack.takeLast();
            int numOfTranpose = QString(tmp[1]).toInt();
            GraphTree result = inputMain.operation_M(numOfTranpose);
            valueStack.append( result );
            continue;
        }
        QStringList newValue = {tmp};
        qDebug() << newValue;
        valueStack.append( GraphTree( newValue ) );
    }
    return valueStack.last().getStringRepresentation();
}

int GraphTree::getWidth()
{
    int maxSize = 0;

    for(int i=0; i < representation.size();i++) {
        if( representation[i].size() > maxSize){
            maxSize = representation[i].size();
        }
    }
    return maxSize;
}

int GraphTree::getHeigth()
{
    return representation.size();
}

GraphTree GraphTree::operator+(GraphTree adding)
{
    const int numOfSpaceSeparators = 3;
    QStringList newGraph;

    int height_1 = this->getHeigth();
    int height_2 = adding.getHeigth();

    int newHeignt = height_1;
    if(height_1 < height_2){
        newHeignt = height_2;
    }

    int width_1 = this->getWidth();
    int width_2 = adding.getWidth();
    int newWidth = width_1 + width_2 + numOfSpaceSeparators;

    newGraph.append( QString(width_1, ' ') + " ∩ " + QString(width_2, ' ') );
    newGraph.append( QString(newWidth, ' ') );
    for(int i=0; i <newHeignt; i++){
        newGraph.append( this->operator[](i) + QString(numOfSpaceSeparators, ' ') + adding[i] );
    }
    return GraphTree( newGraph );
}

GraphTree GraphTree::operation_Superpose(QVector<GraphTree> adding)
{
    const int numOfSpaceSeparators = 1;
    int newHeight = this->getHeigth();
    int newWidth = this->getWidth();
    for(int i=0; i < adding.size(); i++){
        newWidth += adding[i].getWidth() + numOfSpaceSeparators;
        if( newHeight < adding[i].getHeigth() ){
            newHeight = adding[i].getHeigth();
        }
    }

    QStringList newRepresentation;
    newRepresentation.append( QString(newWidth/2, ' ') + "*");

    for(int i=0; i < newHeight; i++){
        QString newString = "";
        newString += this->operator[](i);
        for(int j=0; j < adding.size(); j++){
            newString += QString(numOfSpaceSeparators, ' ') + adding[j][i];
        }
        newRepresentation.append( newString );
    }
    return GraphTree( newRepresentation );
}

GraphTree GraphTree::operation_M(int index)
{
    int width = this->getWidth()/2;

    QStringList newRepresentation = QStringList(representation);
    newRepresentation.insert(0, QString(width, ' ') + "M" + QString::number(index));

    return GraphTree( newRepresentation );
}

QString GraphTree::operator [](int i)
{
    QString outStr;
    if(i < 0){
        return QString();
    }
    if(i >= representation.size()){
        return QString(this->getWidth(), ' ');;
    }
    QString mainStr = representation[i];
    int offset = this->getWidth() - mainStr.size();
    if(offset != 0){
        outStr += mainStr;
        outStr += QString(offset, ' ');
    }else{
        return mainStr;
    }
    return outStr;
}

QString GraphTree::getStringRepresentation()
{
    QString outString;
    for(int i=0; i <representation.size(); i++){
        outString += representation[i] + "\n";
    }
    return outString;
}
