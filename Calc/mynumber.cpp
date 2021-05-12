#include "mynumber.h"

MyNumber::MyNumber()
{

}

MyNumber::MyNumber(QVector<unsigned int> Vector, int newRank, int newDimension)
{
    rank = newRank;
    dimension = newDimension;
    number = MyNumber::getMatrixFromVector(Vector, rank);
}

MyNumber::MyNumber(QVector<QVector<bool> > Matrix, int newRank, int newDimension)
{
    rank = newRank;
    dimension = newDimension;
    number = Matrix;
}

MyNumber::MyNumber(QString input, int type, int newRank, int newDimension)
{
    rank = newRank;
    dimension = newDimension;
    qDebug() << "MyNumber constructor with QString arg";
    if(type == TYPE_MATRIX){
        QVector< QVector<bool> > newNumber( pow(rank, dimension) );
        for(int i = 0; i < input.size(); i++){
            newNumber[i/rank].append( ((QString)input[i]).toInt() );
        }
        number = newNumber;
    }
    if(type == TYPE_VECTOR){
        QVector< unsigned int > vec;
        QStringList numbers = input.split(" ");
        foreach(QString s, numbers){
            vec.append( s.toInt() );
        }
        number = getMatrixFromVector(vec, rank);
        qDebug() << number << input << vec;
    }
}

QVector<unsigned int> MyNumber::getVectorRepresentation()
{
    return MyNumber::getVectorFromMatrix(number);
}

QVector<QVector<bool> > MyNumber::getMatrixRepresentation()
{
    return number;
}

QVector<bool> MyNumber::getMatrixRepresentationByLongVector()
{
    QVector<bool> out;
    foreach(QVector<bool> v, number){
        out.append(v);
    }
    return out;
}

MyNumber MyNumber::operator_M(int index)
{
    if(index > rank || index <= 0){
        // ASSERT
        return *this;
    }
    QVector<MyNumber> output = this->operator_allM();
    return output[index];
}

QVector<MyNumber> MyNumber::operator_allM()
{
    qDebug() << "operator_allM";
    QVector<MyNumber> output;
    QVector<bool> inputMatrix = getMatrixRepresentationByLongVector();
    int n = dimension;
    int k = rank;
    int len = pow(k, n+1);

    QVector<bool> anotherTmpVec(len);
    int sectorSize = rank;
    int cellSize = rank;
    int width = len/rank;
    int localStepSize = width;

    while(localStepSize != 1){
        int oldStepSize = localStepSize;
        int cellStepSize = width/localStepSize;
        localStepSize /= rank;
        for(int l=0; l< cellStepSize; l++){
            for(int k=0; k< localStepSize; k++){
                for(int i = 0; i< cellSize; i++){
                    for(int j = 0; j < sectorSize; j++){
                        int pos1 = i*localStepSize+j*width+   l*oldStepSize+k;
                        int pos2 = j*localStepSize+i*width+   l*oldStepSize+k;
                        anotherTmpVec[pos1] = inputMatrix[pos2];
                    }
                }
            }
        }
        output.append( MyNumber(VectorRepresentation::getStrFromVectorInt(anotherTmpVec),
                                MyNumber::TYPE_MATRIX,
                                rank, dimension));
    }
    return output;
}

QVector<bool> MyNumber::getPrimaryDecomposition(int matrixRank)
{
    QVector<bool> out;
    if( matrixRank > rank || matrixRank < 0 ){
        return out;
    }

    int sectorSize;
    if(dimension == 0){
        sectorSize = 1;
    }else{
        sectorSize = pow(rank, dimension-1);
    }
    QVector< QVector<bool> > slice = number.mid(matrixRank*sectorSize, sectorSize);
    foreach(QVector<bool> v, slice){
        out.append(v);
    }
    return out;
}


QVector<unsigned int> MyNumber::getVectorFromMatrix( QVector< QVector<bool> > input)
{
    int len = input.size();
    int pseudoRank = input[0].size();
    int sectorSize = len / pseudoRank;

    QVector< QVector<bool> > spacerVec;
    for(int i=0; i < len; i++){
        spacerVec.append( QVector<bool>() );
        for(int j=0; j < pseudoRank; j++){
            spacerVec[i].push_back( bool(0) );
        }
    }
    for(int i =0; i < sectorSize; i++){
        for(int j = 0; j < pseudoRank; j++){
            for(int k = 0; k < pseudoRank; k++){
                spacerVec[i*pseudoRank+j][k] = input[ k*sectorSize+i][ j ];
            }
        }
    }

    QVector<unsigned int> outVec;
    foreach( QVector<bool> matrix, spacerVec ){
        int count = 0;
        unsigned int num = 0;
        foreach(bool b, matrix){
            if(b){
                num += pow(2, count);
            }
            count ++;
        }
        outVec.append(num);
    }
    return outVec;
}

QVector< QVector<bool> > MyNumber::getMatrixFromVector( QVector<unsigned int> input, int rank)
{
    QVector< QVector<bool> > outMatrix;
    foreach( unsigned int num, input){
        QVector<bool> matrixRepresent;
        int i = 0;
        while (num != 0) {
            if(num % 2){
                matrixRepresent.append(1);
            }else{
                matrixRepresent.append(0);
            }
            num = num >> 1;
            i++;
        }
        for(; i < rank; i++){
            matrixRepresent.append(0);
        }
        outMatrix.append( matrixRepresent );
    }

    int len = outMatrix.size();
    int pseudoRank = outMatrix[0].size();
    QVector< QVector<bool> > magicOut;
    for(int i=0; i < len; i++){
        magicOut.append( QVector<bool>() );
        for(int j=0; j < pseudoRank; j++){
            magicOut[i].push_back( bool(0) );
        }
    }
    int count = 0;
    for(int j = 0; j < pseudoRank; j++){
        for(int i =0; i < len; i++){
                magicOut[count/pseudoRank][count%pseudoRank] = outMatrix[i][j];

                qDebug() << "-+-" << count/pseudoRank <<  count%pseudoRank;
                qDebug() << i << j;
                count++;
        }
    }

    return magicOut;
}

int MyNumber::getStringLen(QString str, int type)
{
qDebug() << str << type << "getStringLen";
    if(type == TYPE_MATRIX){
        qDebug() << "MATRIX";
        return str.length();
    }
    if(type == TYPE_VECTOR){
        QString tmpStr = str.simplified();
        if(tmpStr.isEmpty()){
            return 0;
        }
        int len = tmpStr.count(" ");
        len += 1;
        return len;
    }
    return 0;
}

QString MyNumber::getStringRepresentation(const int type)
{
    qDebug() << number;
    QString out = "";
    if(type == TYPE_VECTOR){
        QVector<unsigned int> vec = getVectorRepresentation();
        foreach(unsigned int num, vec){
            out += QString::number( num );
        }
    }
    if(type == TYPE_MATRIX){
        foreach(QVector<bool> matrix, number){
            foreach(bool b, matrix){
                out += QString::number(b);
            }
        }
    }
    return out;
}

QString MyNumber::getExtendetStringRepresentation(const int type, int offset = 0)
{
    qDebug() << number;
    int splitter;
    if(dimension == 0){
        splitter = rank;
    }else{
        splitter = pow(rank, dimension-1);
    }
    QString out = "";
    if(type == TYPE_VECTOR){
        out += "(";
        QVector<unsigned int> vec = getVectorRepresentation();
        int count = 1;
        for(QVector<unsigned int>::Iterator i = vec.begin();
            i != vec.end();
            i++){
            if((i+1) == vec.end()){
                out += QString::number( *i );
                continue;
            }else if (count % splitter == 0) {
                out += QString::number( *i );
                out += "   ";
            }else{
                out += QString::number( *i ) + " ";
            }
            count ++;
        }
        out += ")";
    }
    if(type == TYPE_MATRIX){
        int count = 0;
        foreach(QVector<bool> matrix, number){
            if(count != 0 &&
                    count % splitter == 0){
                out += "\n" + QString(offset, QChar(' '));
            }
            foreach(bool b, matrix){
                out += QString::number(b);
            }
            out += " ";
            count ++;
        }
    }
    return out;
}
