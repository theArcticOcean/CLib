#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLight.h>
#include <vtkCamera.h>
#include <vtkActor2D.h>
#include <vtkTextActor.h>
#include <vtkProperty2D.h>
#include <vtkTextProperty.h>
#include <vtkBoxWidget.h>
#include <vtkContourWidget.h>
#include <vtkImageActor.h>

namespace Ui {
class Widget;
}

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();
#define CPP_SET_MACRO(name,type) \
    virtual void Set##name(type _arg) \
  { \
    if (this->name != _arg) \
    { \
    this->name = _arg; \
    } \
  }

#define CPP_GET_MACRO(name,type) \
    virtual type Get##name() {\
    return this->name; \
    }


class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_OutputButton_clicked();

private:
    Ui::Widget *ui;
    vtkSPtr<vtkContourWidget> m_ContourWidget;
    vtkSPtr<vtkActor> m_Actor;
    vtkSPtr<vtkImageActor> m_Image;
};

#endif // WIDGET_H
