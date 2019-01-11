#ifndef Window_H
#define Window_H

#include <QWidget>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkPlane.h>

#include "Tools.h"

#define vSP vtkSmartPointer
#define vSPNew(Var, Type)    vSP<Type> Var = vSP<Type>::New();

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_verticalSlider_valueChanged(int value);

private:
    void loadSTLFile();

    Ui::Widget *ui;
    vSP<vtkRenderer> m_Renderer;
    vSP<vtkRenderWindow> m_RenderWindow;
    vSP<vtkPlane> m_ClipPlane;
    PointStruct m_ClipPlaneNormal;
};

#endif // Window_H
