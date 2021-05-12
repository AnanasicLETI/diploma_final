#include "helpdialog.h"
#include "ui_helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Помощь");
    QFile f(":/helpTest/help.txt");
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    QString help = f.readAll();

    ui->helpTextBrowser->setHtml(help);
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::on_pushButton_clicked()
{
    delete this;//destroy();
}
