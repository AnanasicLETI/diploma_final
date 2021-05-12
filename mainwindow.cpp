#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    isValidInput = false;
    state = 4;
    ui->tabWidget->setCurrentIndex(state);

    QObject::connect(this, SIGNAL( updateSignal() ), this, SLOT( updateInformation()) );

    MatrixCalc::setDimension( ui->dimensionSpinBox->value() );
    MatrixCalc::setRank( ui->RankSpinBox->value() );

    termListEditors = new qTermListManager(this);
    termListEditors->setLayout( ui->sequenceLayout_4 );
    ui->scrollTermLinesArea->setLayout( ui->sequenceLayout_4 );

    input_3_ListEditors = new qTermListManager(this);
    input_3_ListEditors->setLayout( ui->input3_InternalLayout );
    input_3_ListEditors->setInputType(MyNumber::TYPE_MATRIX);

    updateInformation();
}

MainWindow::~MainWindow()
{
    delete termListEditors;
    delete ui;
}

void MainWindow::errorMesage(QString str)
{
    QMessageBox msgBox;
    msgBox.setText(str);
    msgBox.exec();
}

void MainWindow::update_q_Superpose_list()
{
    int dim = MatrixCalc::getDimension();
    QStringList stringList;
    for (int i=0; i<dim; i++) {
        stringList.append("m" + QString::number(i));
    }
    qDebug() << stringList;
    input_3_ListEditors->updateByNew_q_termList(stringList);
}

void MainWindow::on_RankSpinBox_valueChanged(int rank)
{
    MatrixCalc::setRank(rank);
    updateInformation();
}

void MainWindow::on_dimensionSpinBox_valueChanged(int dimension)
{
    MatrixCalc::setDimension(dimension);
    updateInformation();
}

void MainWindow::on_caclFirstBtn_clicked()
{
    if(int errorCodes = inputValidation(state)){
        QMessageBox msgBox;
        msgBox.setText("Некорректный ввод\n"+ErrorManager::getExtendedErrString_ByErrCode(errorCodes));
        msgBox.exec();
        return;
    }

    QString outStr;

    switch (state) {
    case 0:{
        outStr = MatrixCalc::getMatrixFormFromVectorInput( MyNumber(ui->input1->toPlainText(),
                                                       MyNumber::TYPE_VECTOR,
                                                       MatrixCalc::getRank(),
                                                       MatrixCalc::getDimension()) );
    }break;
    case 1:{
        outStr = MatrixCalc::getVectorFormFromMatrixInput( MyNumber(ui->input2->toPlainText(),
                                                        MyNumber::TYPE_MATRIX,
                                                        MatrixCalc::getRank(),
                                                        MatrixCalc::getDimension()) );
    }break;
    case 2:{
        MatrixCalc::getSuperpose(MyNumber(ui->input3_External->text(),
                                                MyNumber::TYPE_MATRIX,
                                                MatrixCalc::getRank(),
                                                MatrixCalc::getDimension() ),
                                        QVector<MyNumber>::fromList(input_3_ListEditors->getMatrixTermsWithValues().values()),
                                        &outStr);
    }break;
    case 3:{
//        QVector<unsigned int> input = calc::getVectorIntFromStr();
        MatrixCalc::getTranspose(MyNumber(ui->input4->toPlainText(),
                                                MyNumber::TYPE_MATRIX,
                                                MatrixCalc::getRank(),
                                                MatrixCalc::getDimension()),
                                        &outStr);
    }break;
    case 4:{

         QMap<QString, MyNumber> inputVectors;
        if( ui->vectorInputRadioBtn->isChecked() ){
            inputVectors = termListEditors->getVectorTermsWithValues();
        }else{
            inputVectors = termListEditors->getMatrixTermsWithValues();
        }
        outStr = MatrixCalc::calcFifth( &inputVectors,
                                    ui->formulaLineEdit->text());

        QStringList tmp =  PolandParser::getPolandParse( ui->formulaLineEdit->text() );
    }break;
    }

    ui->outputPlainTextEdit->setPlainText(outStr);

}



void MainWindow::updateInformation()
{
    qDebug()<< "MainWindow::updateInformation" ;

    unsigned int requiredInputLength = MatrixCalc::getMatrixLen(); //matrixCalc::getInputLength();
    unsigned int maxElementValue = MatrixCalc::getVectorMaxElement();

    QString errorStr = "";
    inputValidation(state, &errorStr);
    switch (state) {
    case 0:{
        QString text = ui->input1->toPlainText();
        int size = text.split(" ").size();
        ui->input1CountLbl->setText("Вводимая длина: "+QString::number(size));
        requiredInputLength = MatrixCalc::getVectorLen();
        maxElementValue= MatrixCalc::getVectorMaxElement();
    }break;
    case 1:{
        ui->input2CountLbl->setText("Вводимая длина: "+QString::number(ui->input2->toPlainText().size()));
        requiredInputLength = MatrixCalc::getMatrixLen();
        maxElementValue= 1;
    }break;
    case 2:{
        update_q_Superpose_list();
        ui->input3CountLbl->setText("Длина внешней последовательности: "+QString::number(ui->input3_External->text().size()));
        requiredInputLength = MatrixCalc::getMatrixLen();
        maxElementValue= 1;
    }break;
    case 3:{
        ui->input4CountLbl->setText("Вводимая длина: "+QString::number(ui->input4->toPlainText().size()));
        requiredInputLength = MatrixCalc::getMatrixLen();
        maxElementValue= 1;
    }break;
    case 4:{
        termListEditors->updateByNew_q_termList( FormulaManager::get_q_WithIndex( ui->formulaLineEdit->text()) );

        if( ui->vectorInputRadioBtn->isChecked() ){
            maxElementValue= qPow(2, ui->RankSpinBox->value()) - 1;
            requiredInputLength  = qPow(ui->RankSpinBox->value(), (ui->dimensionSpinBox->value()));
        }else{
            maxElementValue= 1;
        }
    }break;
    }
    ui->errorLable->setText(errorStr);
    ui->statusbar->showMessage("Необходимая длина: "+ QString::number(requiredInputLength)
            +" Максимальный элемент: "+QString::number(maxElementValue)
            +"");
}




int MainWindow::inputValidation(int localState, QString *errorStr)
{
    int errorCode = 0;
    int type;
    bool isStrFake = false;
    if( ui->vectorInputRadioBtn->isChecked() ){
        type = MyNumber::TYPE_VECTOR;
    }else{
        type = MyNumber::TYPE_MATRIX;
    }
    qDebug() << " MainWindow::inputValidation(int state)" << localState;
    if(errorStr == nullptr){
        errorStr = new QString();
        isStrFake = true;
    }
    switch (localState) {
    case 0:{
        errorCode = ValidationManager::validateFirst(ui->input1,
                                                        errorStr);
    }break;
    case 1:{
        errorCode = ValidationManager::validateSecond(ui->input2,
                                                        errorStr);
    }break;
    case 2:{
        errorCode = ValidationManager::validateThird(ui->input3_External,
                                                        input_3_ListEditors,
                                                        errorStr);
    }break;
    case 3:{
        errorCode = ValidationManager::validateFouth(ui->input4,
                                                        errorStr);
    }break;
    case 4:{
        errorCode = ValidationManager::validateFifth(ui->formulaLineEdit,
                                                        termListEditors,
                                                        errorStr,
                                                        type);
    }break;
    }
    if( errorCode ){
        isValidInput = false;
    }else{
        isValidInput = true;
    }
    if( isStrFake ){
        delete errorStr;
    }
    return errorCode;
}

void MainWindow::saveLogFile(QString prefix, QString data)
{
    QFile file(prefix+"_"+ QDateTime::currentDateTime().toString("MM_dd_hh_mm_ss") +".txt");
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    stream.setCodec("UTF-16");
    stream << data;
    stream.flush();
    file.close();
}

void MainWindow::on_saveBtn_clicked()
{
    if(ui->outputPlainTextEdit->toPlainText().isEmpty()){
        QMessageBox msgBox;
        msgBox.setText("Некорректный ввод");
        msgBox.exec();
        return;
    }

    QString outStr;

    switch (state) {
    case 0:{
        outStr = "Ввод:\n" + MyNumber(ui->input1->toPlainText(),
                                       MyNumber::TYPE_VECTOR,
                                       MatrixCalc::getRank(),
                                       MatrixCalc::getDimension())
                                .getExtendetStringRepresentation(MyNumber::TYPE_VECTOR, 0)
                + "\n";
    }break;
    case 1:{
        outStr = "Ввод:\n" +  MyNumber(ui->input2->toPlainText(),
                                       MyNumber::TYPE_MATRIX,
                                       MatrixCalc::getRank(),
                                       MatrixCalc::getDimension())
                .getExtendetStringRepresentation(MyNumber::TYPE_MATRIX, 0)
                 + "\n";
    }break;
    case 2:{
        outStr = "Ввод:\nВнешняя последовательность:\n" + MyNumber(ui->input3_External->text(),
                                                                   MyNumber::TYPE_MATRIX,
                                                                   MatrixCalc::getRank(),
                                                                   MatrixCalc::getDimension())
                                            .getExtendetStringRepresentation(MyNumber::TYPE_MATRIX, 0)
                                            + "\n";
        outStr += "Внутренняя последовательность:\n" +
                input_3_ListEditors->getStringWithValuesForRepresentation(MyNumber::TYPE_MATRIX)
                + "\n";
    }break;
    case 3:{
        outStr = "Ввод:\n" + MyNumber(ui->input4->toPlainText(),
                                      MyNumber::TYPE_MATRIX,
                                      MatrixCalc::getRank(),
                                      MatrixCalc::getDimension())
               .getExtendetStringRepresentation(MyNumber::TYPE_MATRIX, 0)
                + "\n";
    }break;
    case 4:{
        int type;
        if( ui->vectorInputRadioBtn->isChecked() ){
            type = MyNumber::TYPE_VECTOR;
        }else{
            type = MyNumber::TYPE_MATRIX;
        }
        outStr = "Вводимый терм:\nt=" + ui->formulaLineEdit->text() + "\n";
        outStr += "Вводимые мультиоперации: \n";
        outStr += termListEditors->getStringWithValuesForRepresentation(type);
        outStr += "\nВычисления.\n";
        outStr += "Граф выполнения метаопераций: \n";
        QStringList tmp =  PolandParser::getPolandParse( ui->formulaLineEdit->text() );
        QString graph = GraphTree::getFormulaGraphTree( &tmp, ui->dimensionSpinBox->value() );
        outStr += graph + "\n\n";
        outStr += "Выполнение вычисления в вершинах графа:\n";
        outStr += ui->outputPlainTextEdit->toPlainText();
    }break;
    }
    if(state != 4){
        outStr += "Вычисления.\n"+ui->outputPlainTextEdit->toPlainText();
    }



    QString fileName = QFileDialog::getSaveFileName(nullptr, "Сохранение файла", "", "*.txt");

    if (fileName != "") {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            errorMesage("Неверное имя файла");
        } else {
            QTextStream stream(&file);
            stream.setCodec("UTF-16");
            stream << outStr;
            stream.flush();
            file.close();
        }
    }

}

void MainWindow::on_vectorInputRadioBtn_clicked()
{
    updateInformation();
}


void MainWindow::on_input1_textChanged()
{
    updateInformation();
}

void MainWindow::on_input2_textChanged()
{
    updateInformation();
}

void MainWindow::on_input3_External_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    updateInformation();
}

void MainWindow::on_input3_Internal_textChanged()
{
    updateInformation();
}

void MainWindow::on_input4_textChanged()
{
    updateInformation();
}

void MainWindow::on_matrixInputRadioBtn_clicked()
{
    updateInformation();
}

void MainWindow::updateInformation(const QString &arg1)
{
    qDebug()<< "MainWindow::updateInformation" ;
    Q_UNUSED(arg1);
    updateInformation();
}

void MainWindow::on_pushButton_clicked()
{
    QString text = ui->formulaLineEdit->text();
    int pos = ui->formulaLineEdit->cursorPosition();
    text = text.mid(0, pos) + "⋂" + text.mid(pos);
    ui->formulaLineEdit->setText(text);
}

void MainWindow::on_helpAction_triggered()
{
   HelpDialog *hD = new HelpDialog();
   hD->setModal(true);
   hD->exec();
}


void MainWindow::on_formulaLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    updateInformation();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    state = index;
    emit updateSignal();
}

void MainWindow::on_matrixInputRadioBtn_toggled(bool isMatrix)
{
    if( !isMatrix ){
        termListEditors->setInputType(MyNumber::TYPE_VECTOR);
    }else{
        termListEditors->setInputType(MyNumber::TYPE_MATRIX);
    }
}
