#ifndef QTERMLISTMANAGER_H
#define QTERMLISTMANAGER_H

#include <QObject>
#include <QStringList>
#include <QVBoxLayout>
#include <QDebug>

#include "Calc/vectorRepresentation.h"
#include "Calc/matrixcalc.h"
#include "Calc/mynumber.h"
#include "qtermwidget.h"

class qTermListManager: public QObject
{
    Q_OBJECT
public:
    qTermListManager(QObject *mainWindowPtr);
    ~qTermListManager();
    void updateByNew_q_termList(QStringList q_termList);
    void setLayout(QLayout *newLayout);
    QVBoxLayout *getLayout();

    QVector<qTermWidget*> termWidgetList;
    QMap<QString, MyNumber> getMatrixTermsWithValues();
    QMap<QString, MyNumber> getVectorTermsWithValues();


    QString getStringWithValuesForRepresentation(int type);

    void setInputType(int type);


private:

    QMap<QString, MyNumber> getTermsWithValues(int type);
    void deleteWidgetFromLayout(qTermWidget *w);

    QVBoxLayout *layout;
    QObject  *mainWindow;
    qTermWidget *activeWidget;
    int inputType;

public slots:
    void activeObjectChanged(qTermWidget *obj);
    void lineEditUpdate(QString);
signals:
    void update();
};

#endif // QTERMLISTMANAGER_H
