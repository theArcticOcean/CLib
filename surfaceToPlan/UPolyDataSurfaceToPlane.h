#pragma once

#include <vtkPolyDataAlgorithm.h>
#include <vtkPlane.h>
#include <vtkCell.h>
#include <vtkPlane.h>

class CUPolyDataSurfaceToPlane : public vtkPolyDataAlgorithm
{
public:
  vtkTypeMacro(CUPolyDataSurfaceToPlane, vtkPolyDataAlgorithm);
  static CUPolyDataSurfaceToPlane* New();
  void SetClipPlane(vtkPlane *plane);

protected:
  CUPolyDataSurfaceToPlane(vtkPlane* cf = nullptr);
  ~CUPolyDataSurfaceToPlane() override;
  vtkSmartPointer<vtkPlane> GetCellPlane(vtkSmartPointer<vtkCell> cell, bool &outOfPlane);
  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  vtkPlane* m_ClipFunction;

private:
  CUPolyDataSurfaceToPlane(const CUPolyDataSurfaceToPlane&) = delete;
  void operator=(const CUPolyDataSurfaceToPlane&) = delete;
};
