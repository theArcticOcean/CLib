/**
 * @class   CUVtkSelectPolyData
 * @brief   Similar to vtkSelectPolyData, select portion of polygonal mesh;
 *          generate selection scalars
*/

#ifndef CUVtkSelectPolyData_h
#define CUVtkSelectPolyData_h

#include "vtkFiltersModelingModule.h" // For export macro
#include "vtkPolyDataAlgorithm.h"
#include "UMacroDefinition.h"
#include "Tools.h"

#define VTK_INSIDE_SMALLEST_REGION 0
#define VTK_INSIDE_LARGEST_REGION 1
#define VTK_INSIDE_CLOSEST_POINT_REGION 2

class vtkCharArray;
class vtkPoints;
class vtkIdList;

namespace SelectRegion {
    struct CellInfo
    {
        PointStruct normal;
        PointStruct pts[3];
        CellInfo operator=(CellInfo anotherCell)
        {
            normal = anotherCell.normal;
            pts[0] = anotherCell.pts[0];
            pts[1] = anotherCell.pts[1];
            pts[2] = anotherCell.pts[2];
            return *this;
        }
        // heron math way
        double GetCellArea()
        {
            double ds1 = (pts[0] - pts[1]).Length();
            double ds2 = (pts[0] - pts[2]).Length();
            double ds3 = (pts[1] - pts[2]).Length();
            double p = (ds1 + ds2 + ds3) / 2;
            double S = sqrt(p * (p - ds1) * (p - ds2) * (p - ds3));
            return S;
        }
    };
}

class CUVtkSelectPolyData : public vtkPolyDataAlgorithm
{
public:
  /**
   * Instantiate object with InsideOut turned off, and
   * GenerateSelectionScalars turned off. The unselected output
   * is not generated, and the inside mode is the smallest region.
   */
  static CUVtkSelectPolyData *New();

  vtkTypeMacro(CUVtkSelectPolyData,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Set/Get the flag to control behavior of the filter. If
   * GenerateSelectionScalars is on, then the output of the filter
   * is the same as the input, except that scalars are generated.
   * If off, the filter outputs the cells laying inside the loop, and
   * does not generate scalars.
   */
  vtkSetMacro(GenerateSelectionScalars,vtkTypeBool);
  vtkGetMacro(GenerateSelectionScalars,vtkTypeBool);
  vtkBooleanMacro(GenerateSelectionScalars,vtkTypeBool);
  //@}

  //@{
  /**
   * Set/Get the InsideOut flag. When off, the mesh within the loop is
   * extracted. When on, the mesh outside the loop is extracted.
   */
  vtkSetMacro(InsideOut,vtkTypeBool);
  vtkGetMacro(InsideOut,vtkTypeBool);
  vtkBooleanMacro(InsideOut,vtkTypeBool);
  //@}

  //@{
  /**
   * Set/Get the array of point coordinates defining the loop. There must
   * be at least three points used to define a loop.
   */
  virtual void SetLoop(vtkPoints*);
  vtkGetObjectMacro(Loop,vtkPoints);
  //@}

  CPP_GET_MACRO( m_PointMarks, vtkIntArray * )

  //@{
  /**
   * Set/Get the point used in SelectionModeToClosestPointRegion.
   */
  vtkSetVector3Macro(ClosestPoint,double);
  vtkGetVector3Macro(ClosestPoint,double);
  //@}

  //@{
  /**
   * Control how inside/outside of loop is defined.
   */
  vtkSetClampMacro(SelectionMode,int,
             VTK_INSIDE_SMALLEST_REGION,VTK_INSIDE_CLOSEST_POINT_REGION);
  vtkGetMacro(SelectionMode,int);
  void SetSelectionModeToSmallestRegion()
    {this->SetSelectionMode(VTK_INSIDE_SMALLEST_REGION);};
  void SetSelectionModeToLargestRegion()
    {this->SetSelectionMode(VTK_INSIDE_LARGEST_REGION);};
  void SetSelectionModeToClosestPointRegion()
    {this->SetSelectionMode(VTK_INSIDE_CLOSEST_POINT_REGION);};
  const char *GetSelectionModeAsString();
  //@}

  //@{
  /**
   * Control whether a second output is generated. The second output
   * contains the polygonal data that's not been selected.
   */
  vtkSetMacro(GenerateUnselectedOutput,vtkTypeBool);
  vtkGetMacro(GenerateUnselectedOutput,vtkTypeBool);
  vtkBooleanMacro(GenerateUnselectedOutput,vtkTypeBool);
  vtkGetMacro(m_BrokenPtId,vtkIdType);
  //@}

  /**
   * Return output that hasn't been selected (if GenreateUnselectedOutput is
   * enabled).
   */
  vtkPolyData *GetUnselectedOutput();

  /**
   * Return the (mesh) edges of the selection region.
   */
  vtkPolyData *GetSelectionEdges();

  // Overload GetMTime() because we depend on Loop
  vtkMTimeType GetMTime() override;

protected:
  CUVtkSelectPolyData();
  ~CUVtkSelectPolyData() override;

  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  void HandleSelectPtScalars( vtkPolyData *output );

  vtkTypeBool GenerateSelectionScalars;
  vtkTypeBool InsideOut;
  vtkPoints *Loop;
  int SelectionMode;
  double ClosestPoint[3];
  vtkTypeBool GenerateUnselectedOutput;
  vtkIdType m_BrokenPtId;
  vtkIntArray* m_PointMarks;
  std::vector<bool> m_ValidPointIds;

private:
  vtkPolyData *Mesh;
  void GetPointNeighbors (vtkIdType ptId, vtkIdList *nei);

private:
  CUVtkSelectPolyData(const CUVtkSelectPolyData&) = delete;
  void operator=(const CUVtkSelectPolyData&) = delete;
};

//@{
/**
 * Return the method of determining in/out of loop as a string.
 */
inline const char *CUVtkSelectPolyData::GetSelectionModeAsString(void)
{
  if ( this->SelectionMode == VTK_INSIDE_SMALLEST_REGION )
  {
    return "InsideSmallestRegion";
  }
  else if ( this->SelectionMode == VTK_INSIDE_LARGEST_REGION )
  {
    return "InsideLargestRegion";
  }
  else
  {
    return "InsideClosestPointRegion";
  }
}
//@}

#endif
