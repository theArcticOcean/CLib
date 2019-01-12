#ifndef Window_H
#define Window_H

#include <QWidget>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkPlane.h>
#include <vtkClipPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkImplicitPlaneWidget.h>
#include <vtkSTLReader.h>

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
    void clipSetting();

    Ui::Widget *ui;
    vSP<vtkSTLReader> m_STLReader;
    vSP<vtkRenderer> m_Renderer;
    vSP<vtkRenderWindow> m_RenderWindow;
    vSP<vtkPlane> m_ClipPlane;
    vSP<vtkActor> m_ArchActor;
    vSP<vtkActor> m_SelectActor;
    vSP<vtkClipPolyData> m_Clipper;
    vSP<vtkPolyDataMapper> m_SelectMapper;
    vSP<vtkImplicitPlaneWidget> m_PlaneWidget;
    PointStruct m_ClipPlaneNormal;
    int m_LastValue;
    double m_ZLength;
};

#endif // Window_H
