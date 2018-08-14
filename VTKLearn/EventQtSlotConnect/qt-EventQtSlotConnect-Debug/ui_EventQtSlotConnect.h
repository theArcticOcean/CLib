/********************************************************************************
** Form generated from reading UI file 'EventQtSlotConnect.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EVENTQTSLOTCONNECT_H
#define UI_EVENTQTSLOTCONNECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include "QVTKOpenGLWidget.h"

QT_BEGIN_NAMESPACE

class Ui_EventQtSlotConnect
{
public:
    QAction *actionOpenFile;
    QAction *actionExit;
    QAction *actionPrint;
    QAction *actionHelp;
    QAction *actionSave;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QVTKOpenGLWidget *qvtkWidget;

    void setupUi(QMainWindow *EventQtSlotConnect)
    {
        if (EventQtSlotConnect->objectName().isEmpty())
            EventQtSlotConnect->setObjectName(QStringLiteral("EventQtSlotConnect"));
        EventQtSlotConnect->resize(561, 575);
        actionOpenFile = new QAction(EventQtSlotConnect);
        actionOpenFile->setObjectName(QStringLiteral("actionOpenFile"));
        actionOpenFile->setEnabled(true);
        actionExit = new QAction(EventQtSlotConnect);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionPrint = new QAction(EventQtSlotConnect);
        actionPrint->setObjectName(QStringLiteral("actionPrint"));
        actionHelp = new QAction(EventQtSlotConnect);
        actionHelp->setObjectName(QStringLiteral("actionHelp"));
        actionSave = new QAction(EventQtSlotConnect);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        centralwidget = new QWidget(EventQtSlotConnect);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        qvtkWidget = new QVTKOpenGLWidget(centralwidget);
        qvtkWidget->setObjectName(QStringLiteral("qvtkWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(qvtkWidget->sizePolicy().hasHeightForWidth());
        qvtkWidget->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(qvtkWidget);

        EventQtSlotConnect->setCentralWidget(centralwidget);

        retranslateUi(EventQtSlotConnect);

        QMetaObject::connectSlotsByName(EventQtSlotConnect);
    } // setupUi

    void retranslateUi(QMainWindow *EventQtSlotConnect)
    {
        EventQtSlotConnect->setWindowTitle(QApplication::translate("EventQtSlotConnect", "EventQtSlotConnect", Q_NULLPTR));
        actionOpenFile->setText(QApplication::translate("EventQtSlotConnect", "Open File...", Q_NULLPTR));
        actionExit->setText(QApplication::translate("EventQtSlotConnect", "Exit", Q_NULLPTR));
        actionPrint->setText(QApplication::translate("EventQtSlotConnect", "Print", Q_NULLPTR));
        actionHelp->setText(QApplication::translate("EventQtSlotConnect", "Help", Q_NULLPTR));
        actionSave->setText(QApplication::translate("EventQtSlotConnect", "Save", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class EventQtSlotConnect: public Ui_EventQtSlotConnect {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EVENTQTSLOTCONNECT_H
