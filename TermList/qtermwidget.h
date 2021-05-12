#ifndef QTERMWIDGET_H
#define QTERMWIDGET_H

//#include "TermList/qtermlistmanager.h"

#include <QWidget>
#include <QObject>
#include<QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>

class qTermWidget : public QWidget
{
    Q_OBJECT
public:
    qTermWidget(QString termName = "q", QObject *windowPrt=nullptr);
    ~qTermWidget();
    QString getText();
    bool operator==(QString);

    QLabel termLabel;
    QLineEdit termLineEdit;
    QPushButton termBtn;
    QLabel termInputCounter;

    void setOnClickSignalToContainer(QObject *);

    void clearLineEditCounter();
    void setCounterValue(int value);

private:
    QHBoxLayout termLayout;
    QObject *containerPtr;

private slots:
    void LineEditChanges(int, int);
signals:
    void ActiveWidgetChange(qTermWidget *);
};

#endif // QTERMWIDGET_H
