#include "qtermlistmanager.h"

qTermListManager::qTermListManager(QObject *mainWindowPtr)
{
    layout = new QVBoxLayout();
    termWidgetList = QVector<qTermWidget*>();
    mainWindow = mainWindowPtr;
    QObject::connect(this,
                     SIGNAL( update() ),
                     mainWindow,
                     SLOT( updateInformation() ));
    activeWidget = nullptr;
    inputType = MyNumber::TYPE_VECTOR;
}

qTermListManager::~qTermListManager()
{
    foreach( qTermWidget *w, termWidgetList ){
        deleteWidgetFromLayout(w);
        delete w;
    }
}

void qTermListManager::updateByNew_q_termList(QStringList q_termList)
{
    QVector<QStringList::Iterator> usedIndex;
    QVector< qTermWidget* > deleteVector;
    int index = 0;
    if( !q_termList.isEmpty() ){
        foreach(qTermWidget* w, termWidgetList){
            QStringList::Iterator containedTermIter = std::find_if(q_termList.begin(),
                        q_termList.end(),
                        [w](QString &o){
                                        if( o == (w)->getText() ){
                                            return true;
                                        }return false;
                                    });
            if( containedTermIter != q_termList.end() ){
                usedIndex.append(containedTermIter);
            }else{
                deleteVector.append(w);
            }
        }
    }else{
        deleteVector.append(termWidgetList);
    }

    foreach(qTermWidget* w, deleteVector){
        deleteWidgetFromLayout(w);
        termWidgetList.removeAll(w);
        delete w;
    }
    for(QStringList::Iterator i = q_termList.begin();
        i!=q_termList.end();
        i++){
        bool isContained = std::any_of(usedIndex.begin(),
                    usedIndex.end(),
                    [i](QStringList::Iterator &o){
                                    if(i == o){
                                        return true;
                                    }return false;
                                });
        if(!isContained){
            qTermWidget *newWidget = new qTermWidget(*i, this);
            layout->addWidget( newWidget );
            termWidgetList.append( newWidget );
        }
        index ++;
    }
}

void qTermListManager::setLayout(QLayout *newLayout)
{
    layout = (QVBoxLayout*)newLayout;
}

QVBoxLayout *qTermListManager::getLayout()
{
    return layout;
}

QMap<QString, MyNumber> qTermListManager::getMatrixTermsWithValues()
{
    return getTermsWithValues(MyNumber::TYPE_MATRIX);
}

QMap<QString, MyNumber> qTermListManager::getVectorTermsWithValues()
{
    return getTermsWithValues(MyNumber::TYPE_VECTOR);
}

QMap<QString, MyNumber> qTermListManager::getTermsWithValues(int type)
{
    QMap<QString, MyNumber> out = QMap<QString, MyNumber>();
    foreach(qTermWidget *w, termWidgetList){
        out.insert( w->termLabel.text(),
                    MyNumber(w->termLineEdit.text(),
                             type,
                             MatrixCalc::getRank(),
                             MatrixCalc::getDimension()) );
    }
    return out;
}

QString qTermListManager::getStringWithValuesForRepresentation(int type)
{
    QString out = "";
    foreach(auto x, termWidgetList){
        QString name = x->termLabel.text()+"";
        QString value = MyNumber(x->termLineEdit.text(),
                    type, MatrixCalc::getRank(),
                    MatrixCalc::getDimension())
                .getExtendetStringRepresentation(type, name.length()+QString(": ").length());
        out += name + "= " + value + "\n";
    }
    return out;
}

void qTermListManager::setInputType(int type)
{
    inputType = type;
}

void qTermListManager::deleteWidgetFromLayout(qTermWidget *w)
{
    int index = layout->indexOf(w);
    delete (layout->takeAt(index));
}

void qTermListManager::activeObjectChanged(qTermWidget *newObj)
{
    if(activeWidget == nullptr){
        activeWidget = newObj;
    }
    if( activeWidget != newObj){
        activeWidget->clearLineEditCounter();
        activeWidget = newObj;
    }
}

void qTermListManager::lineEditUpdate(QString)
{
    if(activeWidget != nullptr){
        activeWidget->setCounterValue( MyNumber::getStringLen( activeWidget->termLineEdit.text(),
                                            inputType) );
    }
    emit update();
}

