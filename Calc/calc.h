#ifndef CALC_H
#define CALC_H

#include <QVector>
#include <QString>

class calc{

public:
static QVector<unsigned int> getVectorIntFromStr(QString str)
{
    QVector<unsigned int> out;
    for(int i = 0; i < str.size(); i++){
        out.append( ((QString)str[i]).toInt() );
    }
    return out;
}

template <class T>
static QString getStrFromVectorInt(QVector<T> input)
{
    QString str;
    for(int i=0; i < input.size() ; i++){
        str += QString::number(input[i]);
    }
    return str;
}

};


#endif // CALC_H
