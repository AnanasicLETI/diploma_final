#include "validationmanager.h"


ValidationManager::ValidationManager()
{

}

//QStringList ValidationManager::getUnic_q_StringList(QString formula)
//{
//    return FormulaManager::get_q_WithIndex(formula);
//}

//bool ValidationManager::isValidVector(QString Vector)
//{
//    int len = MatrixCalc::getVectorLen();
//    int vecLen = 0;
//    int maxValue = MatrixCalc::getVectorMaxElement();
//    QStringList numbers = Vector.split(" ");
//    for(const QString &s : qAsConst(numbers)){
//        if(s.isEmpty()){
//            return false;
//        }
//        if( !isDigitalString(s) ){
//            return false;
//        }
//        if( QString(s).toInt() > maxValue || QString(s).toInt() < 0 ){
//            return false;
//        }
//        vecLen ++;
//    }
//    if(vecLen != len){
//        return false;
//    }
//    return true;
//}

//bool ValidationManager::isValidMatrix(QString Matrix)
//{
//    int len = MatrixCalc::getMatrixLen();
//    if( !isDigitalString(Matrix) ){
//        return false;
//    }
//    if(Matrix.length() != len){
//        return false;
//    }
//    foreach(QChar s, Matrix){
//        if(s != "0" || s != "1" ){
//            return false;
//        }
//    }
//    return true;
//}

int ValidationManager::vectorValidateWithErrCode(QString Vector)
{
    int errCode = 0;
    int len = MatrixCalc::getVectorLen();
    int vecLen = 0;
    int maxValue = MatrixCalc::getVectorMaxElement();
    QStringList numbers = Vector.split(" ");
    for(const QString &s : qAsConst(numbers)){
        if(s.isEmpty()){
            errCode |= ERR_WRONG_SYMBOL_IN_SEQUENCE;
            vecLen --;
        }
        if( !isDigitalString(s) ){
            errCode |= ERR_WRONG_SYMBOL_IN_SEQUENCE;
        }
        if( QString(s).toInt() > maxValue || QString(s).toInt() < 0 ){
            errCode |= ERR_SEQUENCE_OUT_FROM_RANGE;
        }
        vecLen ++;
    }
    if(vecLen != len){
        errCode |= ERR_WRONG_SEQUENCE_LEN;
    }
    return errCode;
}

int ValidationManager::matrixValidateWithErrCode(QString Matrix)
{
    int errCode = 0;
    int len = MatrixCalc::getMatrixLen();
    if( !isDigitalString(Matrix) ){
        errCode |= ERR_WRONG_SYMBOL_IN_SEQUENCE;
    }
    if(Matrix.length() != len){
        errCode |= ERR_WRONG_SEQUENCE_LEN;
    }
    foreach(QString s, Matrix){
        if(s != "0" && s != "1" ){
            errCode |= ERR_SEQUENCE_OUT_FROM_RANGE;
        }
    }
    qDebug() << Matrix << len << Matrix.length() << errCode;
    return errCode;
}


bool ValidationManager::isDigitalString(QString str)
{
    for(int i = 0; i < str.size(); i++){
        if(!str[i].isDigit()){
            return false;
        }
    }
    return true;
}

int ValidationManager::validateFirst(QPlainTextEdit *lineEdit, QString *errorMessage)
{
    QString vec = lineEdit->toPlainText();
    unsigned short err = ValidationManager::vectorValidateWithErrCode(vec);
    *errorMessage = ErrorManager::getErrString_ByErrCode(err);
    if(err){
        lineEdit->setStyleSheet("background-color: #FF6859;");
    }else{
        lineEdit->setStyleSheet("");
        *errorMessage = "";
    }
    return err;
}

int ValidationManager::validateSecond(QPlainTextEdit *lineEdit, QString *errorMessage)
{
    QString matrix = lineEdit->toPlainText();
    unsigned short err = ValidationManager::matrixValidateWithErrCode(matrix);
    *errorMessage = ErrorManager::getErrString_ByErrCode( err );
    if(err){
        lineEdit->setStyleSheet("background-color: #FF6859;");
    }else{
        lineEdit->setStyleSheet("");
    }
    return err;
}

int ValidationManager::validateThird(QLineEdit *lineEdit, qTermListManager *termList, QString *errorMessage)
{

    QString strExternal = lineEdit->text();
    unsigned short err = ValidationManager::matrixValidateWithErrCode(strExternal);
    if(err){
        lineEdit->setStyleSheet("background-color: #FF6859;");
    }else{
        lineEdit->setStyleSheet("");
    }

    unsigned int errSquence = validateAllLineEditForSuperpose(termList, MyNumber::TYPE_MATRIX);
    err |= errSquence;

    *errorMessage = ErrorManager::getErrString_ByErrCode( err );
    return err;
}

int ValidationManager::validateFouth(QPlainTextEdit *lineEdit, QString *errorMessage)
{
    QString matrix = lineEdit->toPlainText();
    unsigned short err = ValidationManager::matrixValidateWithErrCode(matrix);
    *errorMessage = ErrorManager::getErrString_ByErrCode( err );
    if(err){
        lineEdit->setStyleSheet("background-color: #FF6859;");
    }else{
        lineEdit->setStyleSheet("");
    }
    return err;
}

int ValidationManager::validateFifth(QLineEdit *formulaLineEdit, qTermListManager *termList, QString *errorMessage, unsigned int type)
{
    QString formula = formulaLineEdit->text();
    unsigned short err = 0;
    if( FormulaManager::isValidFormulaStruct(formula, MatrixCalc::getDimension())
            && ((unsigned int)FormulaManager::getMax_M_index(formula) <= MatrixCalc::getDimension()) ){
        formulaLineEdit->setStyleSheet("");
    }else{
        err = ERR_WRONG_FORMULA;
        formulaLineEdit->setStyleSheet("background-color: #FF6859;");
    }
    unsigned int errSquence = validateAllLineEditForFormula(termList, type );
    err |= errSquence;
    *errorMessage = ErrorManager::getErrString_ByErrCode( err );
    return err;
}

unsigned int ValidationManager::validateAllLineEditForFormula(qTermListManager *termList, unsigned short type)
{
    unsigned short error = 0;
    foreach(qTermWidget *w, termList->termWidgetList){
        unsigned short localError = 0;
        if( type == MyNumber::TYPE_MATRIX ){
            localError = ValidationManager::matrixValidateWithErrCode( w->termLineEdit.text() );
        }
        if( type == MyNumber::TYPE_VECTOR ){
            localError = ValidationManager::vectorValidateWithErrCode( w->termLineEdit.text() );
        }

        if(localError){
            w->termLineEdit.setStyleSheet("background-color: #FF6859;");
        }else{
            w->termLineEdit.setStyleSheet("");
        }
        error |= localError;
    }
    return error;
}

unsigned int ValidationManager::validateAllLineEditForSuperpose(qTermListManager *termList, unsigned short type)
{
    unsigned short error = 0;
    foreach(qTermWidget *w, termList->termWidgetList){
        unsigned short localError = 0;
        if( type == MyNumber::TYPE_MATRIX ){
            localError = ValidationManager::matrixValidateWithErrCode( w->termLineEdit.text() );
        }
        if( type == MyNumber::TYPE_VECTOR ){
            localError = ValidationManager::vectorValidateWithErrCode( w->termLineEdit.text() );
        }

        if(localError){
            w->termLineEdit.setStyleSheet("background-color: #FF6859;");
        }else{
            w->termLineEdit.setStyleSheet("");
        }
        error |= localError;
    }
    if( (unsigned int)termList->termWidgetList.size() != MatrixCalc::getDimension() ){
        error |= ValidationManager::ERR_WRONG_AMOUNT_OF_SEQUENCE;
    }
    return error;
}

