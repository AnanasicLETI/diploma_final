QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QMAKE_LFLAGS += -static

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Calc/mynumber.cpp \
    Help/helpdialog.cpp \
    Managers/errormanager.cpp \
    Managers/formulamanager.cpp \
    Managers/formulavalidator.cpp \
    Managers/graphtree.cpp \
    Managers/validationmanager.cpp \
    main.cpp \
    mainwindow.cpp \
    Calc/matrixcalc.cpp \
    TermList/qtermlistmanager.cpp \
    TermList/qtermwidget.cpp

HEADERS += \
    Calc/mynumber.h \
    Calc/vectorRepresentation.h \
    Help/helpdialog.h \
    Managers/errormanager.h \
    Managers/formulamanager.h \
    Managers/formulavalidator.h \
    Managers/graphtree.h \
    Managers/validationmanager.h \
    mainwindow.h \
    Calc/matrixcalc.h \
    Calc/polandreversparser.h \
    TermList/qtermlistmanager.h \
    TermList/qtermwidget.h

FORMS += \
    Help/helpdialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res/helpText.qrc
