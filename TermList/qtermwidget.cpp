#include "qtermwidget.h"

qTermWidget::qTermWidget(QString termName, QObject *container)
{
    this->setLayout(new QHBoxLayout());
    termLabel.setText(termName);
    termBtn.setText("x");
    this->layout()->addWidget(&termLabel);
    this->layout()->addWidget(&termLineEdit);
    this->layout()->addWidget(&termBtn);
    this->layout()->addWidget(&termInputCounter);
    containerPtr = container;
    termLineEdit.setMaxLength( INT_MAX );
    termLineEdit.setStyleSheet("background-color: #FF6859;");
    QObject::connect(&termLineEdit,
                            SIGNAL( textChanged(QString) ),
                            container,
                            SLOT( lineEditUpdate(QString) ) );
    QObject::connect(&termBtn,
                            SIGNAL( clicked() ),
                            &termLineEdit,
                            SLOT( clear() ) );
    QObject::connect(&termLineEdit,
                     SIGNAL( cursorPositionChanged(int, int) ),
                     this,
                     SLOT( LineEditChanges(int, int) ) );
    QObject::connect(this,
             SIGNAL( ActiveWidgetChange(qTermWidget *) ),
             containerPtr,
             SLOT( activeObjectChanged(qTermWidget *) ) );

}

qTermWidget::~qTermWidget()
{
    QObject::disconnect(this,
         SIGNAL( ActiveWidgetChange(qTermWidget *) ),
         containerPtr,
         SLOT( activeObjectChanged(qTermWidget *) ) );
}

QString qTermWidget::getText()
{
    return termLabel.text();
}

bool qTermWidget::operator==(QString str)
{
    if(termLabel.text() == str){
        return true;
    }
    return false;
}

//void qTermWidget::setOnClickSignalToContainer(QObject *container)
//{
//    if(container != nullptr){
//        QObject::disconnect(this,
//             SIGNAL( ActiveWidgetChange(qTermWidget *) ),
//             container,
//             SLOT( activeObjectChanged(qTermWidget *) ) );
//    }
//    QObject::connect(this,
//         SIGNAL( ActiveWidgetChange(qTermWidget *) ),
//         container,
//                     SLOT( activeObjectChanged(qTermWidget *) ) );
//}

void qTermWidget::clearLineEditCounter()
{
    termInputCounter.clear();
}

void qTermWidget::setCounterValue(int value)
{
    termInputCounter.setText( QString::number(value) );
}

void qTermWidget::LineEditChanges(int, int)
{
    emit ActiveWidgetChange(this);
}
