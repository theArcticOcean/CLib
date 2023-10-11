#pragma once

#include <vtkContextItem.h>
#include <vtkContext2D.h>
#include <vtkObjectFactory.h>
#include <vtkPoints2D.h>
#include <vtkSmartPointer.h>
#include <vtkContextActor.h>
#include <vtkRenderer.h>

struct PaintBrushSettings
{
    double radius = 5;
};

class BrushItem : public vtkContextItem
{
public:
    static BrushItem* New();
    vtkTypeMacro(BrushItem, vtkContextItem);

    virtual bool Paint(vtkContext2D* painter);
    void SetMousePos( double x, double y );
    void SetRadius(double radius);
    vtkSmartPointer<vtkPoints2D> GetCirclePts();
protected:
    BrushItem();

    double m_MousePos[2];
    vtkSmartPointer<vtkPoints2D> m_BrushPts;
};

class UPaintBrush
{
public:
    UPaintBrush();

    void UpdateRenderer(vtkSmartPointer<vtkRenderer> renderer);
    void UpdateItem(double x, double y);
    void CheckPtInsideCircle(double x, double y);
protected:
    PaintBrushSettings m_BrushSettings;
    vtkSmartPointer<vtkContextActor> m_BrushActor;
    vtkSmartPointer<vtkRenderer> m_CurRenderer;
    vtkSmartPointer<vtkPoints2D> m_CriclePts;
};
