#ifndef QTVTKWINDOW_H
#define QTVTKWINDOW_H

#include <QDialog>
#include <vtkSmartPointer.h>
#include <vtkEventQtSlotConnect.h>

namespace Ui {
class QtVTKWindow;
}

class QtVTKWindow : public QDialog
{
    Q_OBJECT

public:
    explicit QtVTKWindow(QWidget *parent = 0);
    ~QtVTKWindow();

private:
    Ui::QtVTKWindow *ui;
    vtkSmartPointer<vtkEventQtSlotConnect> m_VtkConnectQt;
};

#endif // QTVTKWINDOW_H
