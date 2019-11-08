#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <vtkSmartPointer.h>
#include <vtkActor.h>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Dialog *ui;
    vtkSmartPointer<vtkActor> coneActor;
};

#endif // DIALOG_H
