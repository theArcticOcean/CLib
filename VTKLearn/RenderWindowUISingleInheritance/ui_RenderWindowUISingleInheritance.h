/********************************************************************************
** Form generated from reading UI file 'RenderWindowUISingleInheritance.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RENDERWINDOWUISINGLEINHERITANCE_H
#define UI_RENDERWINDOWUISINGLEINHERITANCE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include <QMenuBar>
#include "QVTKOpenGLWidget.h"

QT_BEGIN_NAMESPACE

class Ui_RenderWindowUISingleInheritance
{
public:
    QAction *actionOpenFile;
    QAction *actionExit;
    QAction *actionPrint;
    QAction *actionHelp;
    QAction *actionSave;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menu;
    QVTKOpenGLWidget *qvtkWidget;

    void setupUi(QMainWindow *RenderWindowUISingleInheritance)
    {
        if (RenderWindowUISingleInheritance->objectName().isEmpty())
            RenderWindowUISingleInheritance->setObjectName(QStringLiteral("RenderWindowUISingleInheritance"));
        RenderWindowUISingleInheritance->resize(541, 583);
        menubar = new QMenuBar();
        menu = new QMenu("action");
        actionOpenFile = new QAction(RenderWindowUISingleInheritance);
        actionOpenFile->setObjectName(QStringLiteral("actionOpenFile"));
        actionOpenFile->setEnabled(true);
        actionExit = new QAction(RenderWindowUISingleInheritance);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionPrint = new QAction(RenderWindowUISingleInheritance);
        actionPrint->setObjectName(QStringLiteral("actionPrint"));
        actionHelp = new QAction(RenderWindowUISingleInheritance);
        actionHelp->setObjectName(QStringLiteral("actionHelp"));
        actionSave = new QAction(RenderWindowUISingleInheritance);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        centralwidget = new QWidget(RenderWindowUISingleInheritance);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        qvtkWidget = new QVTKOpenGLWidget(centralwidget);
        qvtkWidget->setObjectName(QStringLiteral("qvtkWidget"));
        qvtkWidget->setGeometry(QRect(10, 20, 511, 541));
        RenderWindowUISingleInheritance->setCentralWidget(centralwidget);

        retranslateUi(RenderWindowUISingleInheritance);

        QMetaObject::connectSlotsByName(RenderWindowUISingleInheritance);
    } // setupUi

    void retranslateUi(QMainWindow *RenderWindowUISingleInheritance)
    {
        RenderWindowUISingleInheritance->setWindowTitle(QApplication::translate("RenderWindowUISingleInheritance", "RenderWindowUISingleInheritance", Q_NULLPTR));
        actionOpenFile->setText(QApplication::translate("RenderWindowUISingleInheritance", "Open", Q_NULLPTR));
        actionExit->setText(QApplication::translate("RenderWindowUISingleInheritance", "e x i t", Q_NULLPTR));
        actionPrint->setText(QApplication::translate("RenderWindowUISingleInheritance", "Print", Q_NULLPTR));
        actionHelp->setText(QApplication::translate("RenderWindowUISingleInheritance", "Help", Q_NULLPTR));
        actionSave->setText(QApplication::translate("RenderWindowUISingleInheritance", "Save", Q_NULLPTR));
        RenderWindowUISingleInheritance->setMenuBar( menubar );
        menubar->addMenu( menu );
        menu->addAction( actionOpenFile );
        menu->addAction( actionExit );
        menu->addAction( actionPrint );
        menu->addAction( actionHelp );
        menu->addAction( actionSave );
    } // retranslateUi

    ~Ui_RenderWindowUISingleInheritance()
    {
        delete actionOpenFile;
        delete actionExit;
        delete actionPrint;
        delete actionHelp;
        delete actionSave;
        delete centralwidget;
        delete menubar;
        delete menu;
//        delete qvtkWidget; // QOpenGLWidget would not be free by us.
    }
};

namespace Ui {
    class RenderWindowUISingleInheritance: public Ui_RenderWindowUISingleInheritance {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENDERWINDOWUISINGLEINHERITANCE_H
