#ifndef POLANDREVERSPARSER_H
#define POLANDREVERSPARSER_H

#include <QStringList>
#include <QDebug>

class PolandParser {

public:
    static QStringList getPolandParse(QString formula, QString *log = nullptr){
        Q_UNUSED(log);
        QStringList outPolandParse;
        QStringList operationsStack;

        QMap<QString, int> priorityMap = {{"⋂", 0}, {"*", 1}};//, {"M", 2}};
        QVector<QString> prefixFunctions = {"M"};

        for(int i =0; i < formula.size(); i++){

            QString readingChar = QString(formula[i]);
            qDebug() << outPolandParse << operationsStack << readingChar;

            if( readingChar == "(" ){
                operationsStack.append("(");
                continue;
            }

            if( readingChar == ")"){
                while(operationsStack.last() != "("){
                    outPolandParse.append( operationsStack.takeLast() );
                }
                operationsStack.removeLast();
                continue;
            }

            if(priorityMap.contains( readingChar ) ||
                    prefixFunctions.contains( readingChar ) ){
                qDebug() << "operationsStack" << operationsStack;
                if(readingChar == "M"){
                    int len = formula.length();
                    i++;
                    while(i < len && formula[i].isDigit() ){
                        readingChar += QString(formula[i]);
                        i++;
                    } i--; // Yes, it's dirty, I'm know it.
                }
                qDebug() << "readingChar"<< readingChar;
                if( operationsStack.isEmpty() || operationsStack.last() == "("){
                    operationsStack.append( readingChar );
                    continue;
                }

                while( prefixFunctions.contains( operationsStack.last() ) ){
                    qDebug() << "containedPrefix";
                    outPolandParse.append( operationsStack.takeLast() );
                }

                if(readingChar[0] == "M"){
                    operationsStack.append(readingChar);
                    continue;
                }

                int priorityOfNewElement = priorityMap.value( readingChar.mid(0,1) );
                int priorityOfLastElement = priorityMap.value( QString(operationsStack.last()[0]) );
// M1(q1*q1)⋂(M1(M1(q2*q2)*q1)⋂M1(q1*q1))
//q1⋂q2*q3
//    qDebug() << "priorityMap" << priorityOfNewElement << priorityOfLastElement << operationsStack.last() << priorityMap;
                if( priorityOfNewElement <= priorityOfLastElement ){
                    outPolandParse.append( operationsStack.takeLast() );
                }
                operationsStack.append(readingChar);
                continue;
            }

            if(readingChar == "q"){
                int len = formula.length();
                i++;
                while(i < len && formula[i].isDigit() ){
                    readingChar += QString(formula[i]);
                    i++;
                }
                outPolandParse.append( readingChar );
                i--; // Yes, it's dirty, I'm know it.
                continue;
            }

            qDebug() << "POLAND PARSE ERROR";

        }
        int tmpLen = operationsStack.size();
        for (int j = 0; j < tmpLen; j++) {
            outPolandParse.append( operationsStack.takeLast() );
        }
        return outPolandParse;
    }
};


#endif // POLANDREVERSPARSER_H
