#include "errormanager.h"

ErrorManager::ErrorManager()
{

}

QString ErrorManager::getErrString_ByErrCode(int errCode)
{
    QString outErr = "";
    if( ValidationManager::ERR_WRONG_SEQUENCE_LEN & errCode ){
        outErr += "ошибка длины строки";
    }
    if( ValidationManager::ERR_WRONG_SYMBOL_IN_SEQUENCE & errCode ){
        if( !outErr.isEmpty() )
            outErr +=", ";
        outErr += "допустим ввод только цифр";
    }
    if( ValidationManager::ERR_SEQUENCE_OUT_FROM_RANGE & errCode ){
        if( !(outErr).isEmpty())
            outErr +=", ";
        outErr += "превышение значений";
    }
    if( ValidationManager::ERR_WRONG_AMOUNT_OF_SEQUENCE & errCode ){
        if( !(outErr).isEmpty())
            outErr +=", ";
        outErr += "неверное количество последовательностей";
    }
    if( ValidationManager::ERR_WRONG_FORMULA & errCode ){
        if( !(outErr).isEmpty())
            outErr +=", ";
        outErr += "неверная формула";
    }
    return outErr;
}

QString ErrorManager::getExtendedErrString_ByErrCode(int errCode)
{
    QString outErr = "";
    if( ValidationManager::ERR_WRONG_SEQUENCE_LEN & errCode ){
        outErr += "Входная строка имеет неправильную длинну";
    }
    if( ValidationManager::ERR_WRONG_SYMBOL_IN_SEQUENCE & errCode ){
        if( !outErr.isEmpty() )
            outErr +="\n";
        outErr += "Входная строка включает неверные символы";
    }
    if( ValidationManager::ERR_SEQUENCE_OUT_FROM_RANGE & errCode ){
        if( !(outErr).isEmpty())
            outErr +="\n";
        outErr += "Значения входной строки выходят за диапазон";
    }
    if( ValidationManager::ERR_WRONG_AMOUNT_OF_SEQUENCE & errCode ){
        if( !(outErr).isEmpty())
            outErr +="\n";
        outErr += "Введено неверное количество последовательностей";
    }
    if( ValidationManager::ERR_WRONG_FORMULA & errCode ){
        if( !(outErr).isEmpty())
            outErr +="\n";
        outErr += "В формуле присутствуют ошибки";
    }
    return outErr;
}
