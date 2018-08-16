#ifndef QTVTKWINDOW_H
#define QTVTKWINDOW_H

#include <QDialog>
#include <vtkSmartPointer.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkNamedColors.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include "doubleViewInteractorStyle.h"

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
    vtkSmartPointer<DoubleViewInteractorStyle> rightInteractorStyle;
    vtkSmartPointer<DoubleViewInteractorStyle> leftInteractorStyle;
    vtkSmartPointer<vtkNamedColors> winBackColor;
};

#endif // QTVTKWINDOW_H
