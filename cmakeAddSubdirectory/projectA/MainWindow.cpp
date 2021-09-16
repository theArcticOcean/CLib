#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Base.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Base obj;
    obj.Say();
}

MainWindow::~MainWindow()
{
    delete ui;
}
