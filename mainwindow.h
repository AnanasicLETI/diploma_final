#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QLineEdit>
#include <QFile>
#include <QTextBrowser>

#include <QDateTime>

#include "Help/helpdialog.h"

#include "Calc/matrixcalc.h"
#include "Calc/polandreversparser.h"
#include "Managers/validationmanager.h"

#include "TermList/qtermwidget.h"
#include "TermList/qtermlistmanager.h"
#include "Calc/vectorRepresentation.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void errorMesage(QString);

    void update_q_Superpose_list();

private slots:
    void on_RankSpinBox_valueChanged(int rank);

    void on_dimensionSpinBox_valueChanged(int arg1);

    void on_caclFirstBtn_clicked();

    void on_input1_textChanged();

    void on_input2_textChanged();

    void on_input3_External_textChanged(const QString &arg1);

    void on_input3_Internal_textChanged();

    void on_input4_textChanged();

    void on_saveBtn_clicked();

    void updateInformation();

    void updateInformation(const QString &arg1);

    void on_vectorInputRadioBtn_clicked();

    void on_matrixInputRadioBtn_clicked();

    void on_pushButton_clicked();

    void on_helpAction_triggered();

    void on_formulaLineEdit_textChanged(const QString &arg1);

    void on_tabWidget_currentChanged(int index);

    void on_matrixInputRadioBtn_toggled(bool checked);

private:
    Ui::MainWindow *ui;

    int state;
    bool isValidInput;

    qTermListManager *termListEditors;
    qTermListManager *input_3_ListEditors;

    int inputValidation(int state, QString *errorStr = nullptr);
    void saveLogFile(QString prefix, QString data);

signals:
    void updateSignal();

};
#endif // MAINWINDOW_H
