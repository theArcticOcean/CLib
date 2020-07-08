#pragma once

#include "math.h"
#include "vtkMath.h"
#include "vtkSmartPointer.h"
#include "vtkMatrix4x4.h"
#include <vtkMassProperties.h>
#include <vtkRenderer.h>
#include "UMacroDefinition.h"


#define vSP vtkSmartPointer     // use SmartPointer whenever you can

extern vtkRenderer *the_Renderer;   // easy debug

#define MAX_DOUBLE 1.79769e+308
#define TINY_FLOAT 1.0e-10f


inline double RadianToDegree(double r) { return r * 180.0/vtkMath::Pi(); } 
inline double DegreeToRadian(double d) { return d * vtkMath::Pi() / 180.0; } 
inline bool Tiny( double f) { return fabs(f)  < TINY_FLOAT; }

static double sZero[3] = {0,0,0};
static double sX[3] = {1,0,0};
static double sY[3] = {0,1,0};
static double sZ[3] = {0,0,1};


UDEF_Class PointStruct
{
public:
	double point[3];
	PointStruct() {}
	PointStruct(double *pt) { point[0]=pt[0]; point[1]=pt[1]; point[2]=pt[2];}
	PointStruct(double x, double y, double z) { point[0]=x; point[1]=y; point[2]=z;}

	void GetPoint(double& x, double& y, double& z) { x = point[0]; y = point[1]; z = point[2]; }
	PointStruct & SetPoint(double x, double y, double z) { point[0] = x; point[1] = y;	point[2] = z; return *this; }
	PointStruct & SetPoint(double p[]) { point[0]=p[0];	point[1]=p[1];	point[2]=p[2];	return *this; }
	PointStruct & Setpoint(float p[]) { point[0]=p[0];	point[1]=p[1];	point[2]=p[2];	return *this; }

	double &operator[](int i) { return point[i]; }
	double operator[](int i) const { return point[i]; }  

    PointStruct operator=(PointStruct p) { SetPoint( p.point); return *this; }
    PointStruct operator+=(const PointStruct &p) { point[0] += p[0];	point[1] += p[1]; point[2] += p[2];	return *this; }
    PointStruct operator-=(const PointStruct &p) { point[0] -= p[0];	point[1] -= p[1]; point[2] -= p[2];	return *this; }
    PointStruct operator*=(double v) { point[0]*=v; point[1]*=v; point[2]*=v;	return *this; }
    PointStruct operator/=(double v) { point[0]/=v; point[1]/=v; point[2]/=v;	return *this; }
    PointStruct operator^=( PointStruct &p) { SetPoint(point[1]*p.point[2]-point[2]*p.point[1], point[2]*p.point[0]-point[0]*p.point[2], point[0]*p.point[1]-point[1]*p.point[0]); return *this; }
    
	PointStruct operator+(double v) { return PointStruct(point[0]+v, point[1]+v, point[2]+v); }
	PointStruct operator-(double v) { return PointStruct(point[0]-v, point[1]-v, point[2]-v); }
	PointStruct operator*(double v) { return PointStruct(point[0]*v, point[1]*v, point[2]*v); }
	PointStruct operator/(double v) { return PointStruct(point[0]/v, point[1]/v, point[2]/v); } 
	PointStruct operator-() { return PointStruct(-point[0], -point[1], -point[2]); }
	double Dot(const PointStruct &p) { return point[0]*p.point[0]+point[1]*p.point[1]+point[2]*p.point[2]; }

	double Length() { return sqrt(point[0]*point[0]+point[1]*point[1]+point[2]*point[2]); }
	PointStruct Unit() { return (operator/=(Length())); }

	friend PointStruct operator*(double v,PointStruct &p) { return p*v; };
	friend PointStruct operator/( PointStruct &p, double v) { return p*(1./v); }
	friend PointStruct operator+(const PointStruct &p1, const PointStruct &p2) { return PointStruct(p1[0]+p2[0],p1[1]+p2[1],p1[2]+p2[2]); }
	friend PointStruct operator-(const PointStruct &p1, const PointStruct &p2) { return PointStruct(p1[0]-p2[0],p1[1]-p2[1],p1[2]-p2[2]); }
	friend double operator*( PointStruct &p1,  PointStruct &p2) { return p1[0]*p2[0]+p1[1]*p2[1]+p1[2]*p2[2]; }
	friend PointStruct operator^(const PointStruct &p1, const PointStruct &p2) { return PointStruct(p1[1]*p2[2]-p1[2]*p2[1],p1[2]*p2[0]-p1[0]*p2[2],p1[0]*p2[1]-p1[1]*p2[0]); }
    friend bool operator==( PointStruct &p1,  PointStruct &p2)
    {
        if( fabs( p1[0] - p2[0] ) < 1e-6 &&
            fabs( p1[1] - p2[1] ) < 1e-6 &&
            fabs( p1[2] - p2[2] ) < 1e-6 )
        {
            return true;
        }
        return false;
    }

    friend bool operator!=( PointStruct &p1,  PointStruct &p2)
    {
        return !( p1 == p2 );
    }
	friend bool operator<( PointStruct &p1,  PointStruct &p2) { return (p1[0] <=  p2[0] && p1[1] <= p2[1] && p1[2] <= p2[2]); }
	friend ostream& operator<<(ostream & os, PointStruct & p) { os << "PointStruct [" << p[0] <<", " << p[1] <<", " << p[2] << "]\n"; return os; }
};


UDEF_Class CPointNormal : public PointStruct
{
public:
	double normal[3];
	CPointNormal() {}
	CPointNormal(double *pt): PointStruct(pt) {}
	CPointNormal(double *pt, double *n) : PointStruct(pt) {normal[0]=n[0]; normal[1]=n[1]; normal[2]=n[2];}
};

UDEF_Class CPointUV : public PointStruct
{
public:
	double m_u, m_v;
	CPointUV() {}
	CPointUV(double *pt, double u, double v): PointStruct(pt) { m_u = u; m_v = v;}
};

UDEF_Class CPointNormalShift : public CPointNormal
{
public:
	double shift[3];
	CPointNormalShift() {}
	CPointNormalShift(double *pt): CPointNormal(pt) {}
	CPointNormalShift(double *pt, double *n): CPointNormal(pt, n) {}
};


inline double Interpolate(double f1, double f2, double w) { return f1*w + f2*(1-w); }
inline void SetVector(double* xyz, double x, double y, double z)	{xyz[0] = x; xyz[1] = y; xyz[2] = z;}
inline void InvertVector( double* xyz)	{xyz[0] = -xyz[0];	xyz[1] = -xyz[1];	xyz[2] = -xyz[2];}
inline void InvertVector(double* dst, double *src) { dst[0] = -src[0]; dst[1] = -src[1]; dst[2] = -src[2]; }
inline void CopyVector(double* dst, double* src)	{ dst[0] = src[0];	dst[1] = src[1]; dst[2] = src[2];}
inline void AddVectors(double* dst, double* src)	{ dst[0] += src[0];	dst[1] += src[1]; dst[2] += src[2];}
inline void Add2Vectors(double* dst, double* s1, double* s2)	{dst[0] = s1[0] + s2[0]; dst[1] = s1[1] + s2[1]; dst[2] = s1[2] + s2[2];}
inline void AddWeightVectors(double* dst, double* src, double w)	{dst[0] += src[0]*w; dst[1] += src[1]*w; dst[2] += src[2]*w;}
inline void Add2VectorsWithWeights(double* dst, double* s1, double* s2, double w1, double w2)	{dst[0] = w1 * s1[0] + w2*s2[0]; \
														dst[1] = w1 * s1[1] + w2*s2[1];	   dst[2] = w1 * s1[2] + w2*s2[2];}
inline void Add3VectorsWithWeights(double* dst, double* s1, double* s2, double* s3, double w1, double w2, double w3)	
				                                        {dst[0] = w1 * s1[0] + w2*s2[0] + w3*s3[0]; 
														 dst[1] = w1 * s1[1] + w2*s2[1] + w3*s3[1];	 dst[2] = w1 * s1[2] + w2*s2[2] + w3*s3[2];}
inline void Sub2Vectors(double* dst, double* s1, double* s2)	{dst[0] = s1[0] - s2[0]; dst[1] = s1[1] - s2[1]; dst[2] = s1[2] - s2[2];}
inline void SubVectors(double* dst, double* src)	{dst[0] -= src[0]; dst[1] -= src[1];	dst[2] -= src[2];}
inline void Avg2Vectors(double* dst, double* s1, double* s2)	{dst[0] = 0.5 * (s1[0] + s2[0]); dst[1] = 0.5 * (s1[1] + s2[1]);													dst[2] = 0.5 * (s1[2] + s2[2]);}
inline void WeightAvg2Vectors(double* dst, double* s1, double* s2, double w1, double w2) { Add2VectorsWithWeights(dst, s1, s2, w1/(w1+w2), w2/(w1+w2)); }
inline double VectorLength( double* xyz) { return sqrt(xyz[0] * xyz[0] + xyz[1] * xyz[1] + xyz[2] * xyz[2]); }
inline double VectorAbsLength( double* xyz) { return abs(xyz[0])  + abs(xyz[1]) + abs(xyz[2]); }
inline double DotVector( double* u, double* v) { return u[0] * v[0] + u[1] * v[1] + u[2] * v[2]; }
inline void ScaleVector(double* xyz, double s)	{xyz[0] *= s; xyz[1] *= s; xyz[2] *= s;}
inline void ScaleVector(double* dst, double* src, double s)	{dst[0] = src[0] * s;	dst[1] = src[1] * s;	dst[2] = src[2] * s;}
inline void CrossVectorBAD(double* u, double* v, double* dst)	{dst[0] = u[1] * v[2] - v[1] * u[2];	dst[1] = v[0] * u[2] - u[0] * v[2];	dst[2] = u[0] * v[1] - v[0] * u[1];}
inline void CrossVector(double* dst, double* u, double* v)	{dst[0] = u[1] * v[2] - v[1] * u[2]; dst[1] = v[0] * u[2] - u[0] * v[2]; dst[2] = u[0] * v[1] - v[0] * u[1];}
inline PointStruct CrossVector(double* u, double* v)	{ PointStruct pt; CrossVector(pt.point,u,v); return pt; }
inline void Intp2Vectors(double* dst, double* s1, double* s2, double w)	{ Add2VectorsWithWeights(dst, s1, s2, w, 1-w); }
inline void WeightAvgNVectors(double* dst, int n, double src[][3], double* srcWeights)	{
														double tw = srcWeights[0];
														for (int i=1; i<n; i++) tw += srcWeights[i];
														ScaleVector(dst, src[0], srcWeights[0]/tw); 
														for (int i=1; i<n; i++) AddWeightVectors(dst, src[i], srcWeights[i]/tw); }
inline void WeightAvg4Vectors(double* dst, double *src1, double w1, double *src2, double w2, double *src3, double w3, double *src4, double w4)	{
														double tw = w1+w2+w3+w4;
														ScaleVector(dst, src1, w1/tw); 
														AddWeightVectors(dst, src2, w2/tw); 
														AddWeightVectors(dst, src3, w3/tw); 
														AddWeightVectors(dst, src4, w4/tw); }
inline void IntpQuadVectors(double* dst, double* p1, double* p2,  double* p3, double* p4, double wu, double wv)
{
	// |  p1__________p2
	// |  |   |       |
	// |  |___|wv	  |
	// |  | wu        |
	// |  |___________|
	//    p3          p4
	double w4 = wu*wv;
	double w3 = (1-wu)*wv;
	double w2 = wu*(1-wv);
	double w1 = (1-wu)*(1-wv);

	dst[0] = w1*p1[0] + w2*p2[0] + w3*p3[0] + w4*p4[0]; 
	dst[1] = w1*p1[1] + w2*p2[1] + w3*p3[1] + w4*p4[1]; 
	dst[2] = w1*p1[2] + w2*p2[2] + w3*p3[2] + w4*p4[2]; 
}
inline double PointsDistanceSquare(double* p1, double* p2) { return	((p1[0] - p2[0]) * (p1[0] - p2[0]) + (p1[1] - p2[1]) * (p1[1] - p2[1]) + (p1[2] - p2[2]) * (p1[2] - p2[2])); }
inline double PointsDistance(double* p1, double* p2) { return sqrt(PointsDistanceSquare(p1,p2)); }
inline bool  CompareVectorsDirection( double* v1, double* v2)	{ return DotVector(v1, v2) > 0.0; }
inline void  Swap(double &d1, double &d2) { double f = d2; d2 = d1; d1 = f; }
inline void  SwapVector(double* v1, double* v2) { double f = v1[0]; v1[0] = v2[0]; v2[0] = f;  f = v1[1]; v1[1] = v2[1]; v2[1] = f; f = v1[2]; v1[2] = v2[2]; v2[2] = f; }
inline void NormalizeVector(double* v)  { ScaleVector(v, 1.0 / VectorLength(v)); }
inline void Normal2Points(double *normal, double* p1, double* p2) {	Sub2Vectors(normal, p2, p1); NormalizeVector( normal ); }
inline void TriNormal(double* normal, double* p1, double* p2, double* p3)
{
	double v1[3], v2[3];

	Sub2Vectors(v1, p2, p1);
	Sub2Vectors(v2, p3, p1);

	CrossVector(normal, v1, v2);
	NormalizeVector(normal);
}
inline vtkMatrix4x4* CreatIdentityMatrix()
{
	vtkMatrix4x4 *Matrix = vtkMatrix4x4::New();
	Matrix->Identity();
	return Matrix;
}

inline bool PointInBox(double *pt, double *border)
{
	return (pt[0] > border[0] && pt[0] < border[1] &&
			pt[1] > border[2] && pt[1] < border[3] &&
			pt[2] > border[4] && pt[2] < border[5]);
}

inline bool BoxInBox(double *box, double *border)
{
	return (box[0] > border[0] && box[1] < border[1] &&
			box[2] > border[2] && box[3] < border[3] &&
			box[4] > border[4] && box[5] < border[5]);
}

inline double AngleCordRadiusRadian(double cord, double radius) { return asin(0.5 * cord / radius ) * 2.0; }
inline double AngleCordRadiusDegree(double cord, double radius) { return RadianToDegree( AngleCordRadiusRadian(cord, radius) ); }

// calculate sweep angle from normal1 to normal2 around axis
// output in the interval [-pi,+pi] radians
inline double GetAngleOf2NormalsRadian(double normal1[], double normal2[], double axis[])
{
	double normal3[3];
	CrossVectorBAD( normal1, normal2, normal3 );
	double d = VectorLength(normal3);
	double theta = asin(d);
	if( DotVector( normal1, normal2) < 0 ) theta = vtkMath::Pi() - theta;
	double sign = DotVector( normal3, axis )> 1.0e-10 ? 1.0 : -1.0; 
	return theta * sign;
}
inline double GetAngleOf2NormalsDegree(double normal1[], double normal2[], double axis[]) { 	return RadianToDegree(GetAngleOf2NormalsRadian(normal1, normal2, axis) ); }


// calculate angle of p1p2p3
// return value [0, pi]
inline double GetTurnAngle3PointsRadian(double p1[], double p2[], double p3[])
{
	double normal1[3], normal2[3];
	Sub2Vectors(normal1, p2, p1);
	NormalizeVector(normal1);
	Sub2Vectors(normal2, p3, p2);
	NormalizeVector(normal2);

	return acos( DotVector(normal1, normal2) );
}
inline double GetTurnAngle3PointsDegree(double p1[], double p2[], double p3[]) { return RadianToDegree(GetTurnAngle3PointsRadian(p1, p2, p3)); }

// calculate angle of p1p2p3 around "axis"
// return value [-pi, pi]
inline double GetSignedTurnAngle3PointsRadian(double p1[], double p2[], double p3[], double axis[])
{
	double normal1[3], normal2[3];
	Normal2Points(normal1, p1, p2);
	Normal2Points(normal2, p2, p3);

	return GetAngleOf2NormalsRadian(normal1, normal2, axis);
}

inline double GetTwoLinesDistance(double A[3], double B[3], double C[3], double D[3]) // Lines AB - CD
{
	if( A[0] == B[0] && A[1] == B[1] && A[2] == B[2] )
		return 0.0;

	if( C[0] == D[0] && C[1] == D[1] && C[2] == D[2] )
		return 0.0;

	double P1 = ( B[0] - A[0] ) * ( B[0] - A[0] ) + ( B[1] - A[1] ) * ( B[1] - A[1] ) + ( B[2] - A[2] ) * ( B[2] - A[2] );
	double P2 = ( B[0] - A[0] ) * ( D[0] - C[0] ) + ( B[1] - A[1] ) * ( D[1] - C[1] ) + ( B[2] - A[2] ) * ( D[2] - C[2] );
	double Q1 =  - ( ( D[0] - C[0] ) * ( B[0] - A[0] ) + ( D[1] - C[1] ) * ( B[1] - A[1] ) + ( D[2] - C[2] ) * ( B[2] - A[2] ) );
	double Q2 =  - ( ( D[0] - C[0] ) * ( D[0] - C[0] ) + ( D[1] - C[1] ) * ( D[1] - C[1] ) + ( D[2] - C[2] ) * ( D[2] - C[2] ) );
	double R1 = ( C[0] - A[0] ) * ( B[0] - A[0] ) + ( C[1] - A[1] ) * ( B[1] - A[1] ) + ( C[2] - A[2] ) * ( B[2] - A[2] );
	double R2 = ( C[0] - A[0] ) * ( D[0] - C[0] ) + ( C[1] - A[1] ) * ( D[1] - C[1] ) + ( C[2] - A[2] ) * ( D[2] - C[2] );
	double m = ( Q2 * R1 - Q1 * R2 ) / ( P1 * Q2 - P2 * Q1 );
	double n = ( P1 * R2 - P2 * R1 ) / ( P1 * Q2 - P2 * Q1 );

	double M[3], N[3];
	M[0] = A[0] + m * ( B[0] - A[0] );
	M[1] = A[1] + m * ( B[1] - A[1] );
	M[2] = A[2] + m * ( B[2] - A[2] );
	N[0] = C[0] + n * ( D[0] - C[0] );
	N[1] = C[1] + n * ( D[1] - C[1] );
	N[2] = C[2] + n * ( D[2] - C[2] );
	return sqrt( ( N[0] - M[0] ) * ( N[0] - M[0] ) + ( N[1] - M[1] ) * ( N[1] - M[1] ) + ( N[2] - M[2] ) * ( N[2] - M[2] ) );
}

inline void GetPointToLineProjection(double A[], double B[], double Point[], double Projection[]) // Line AB, Point, Projection is output
{
	double vectorProjection[3];
	double AB[3], AP[3];
	vtkMath::Subtract(B, A, AB);
	vtkMath::Subtract(Point, A, AP);
	vtkMath::ProjectVector(AP, AB, vectorProjection);
	vtkMath::Add(A, vectorProjection, Projection);
}

inline double GetDistancePoint2Line(double* p, double* l1, double* l2)
{
	double v[3], d[3], r[3];
	Normal2Points(v, l2, l1);
	Sub2Vectors(d, p, l1); 
	CrossVectorBAD(v, d, r);
    return VectorLength(r);
}

inline double GetDistancePoint2LineXY(double* pp, double* ll1, double* ll2)
{
	double p[3] = {pp[0], pp[1], 0};
	double l1[3] = {ll1[0], ll1[1], 0};
	double l2[3] = {ll2[0], ll2[1], 0};

    return GetDistancePoint2Line(p, l1, l2);
}

inline double PolyVolume( vtkPolyData *p)
{
	vSP<vtkMassProperties> vmp = vSP<vtkMassProperties>::New();
	vmp->SetInputData( p );
	return vmp->GetVolume();
}

inline double arctan2Q1Q4( double y, double x )
{
	double abs_y = fabs(y) + TINY_FLOAT;      // kludge to prevent 0/0 condition
	double r = (x - abs_y) / (x + abs_y);
	double angle = vtkMath::Pi() / 4.0 + (0.1963f * r * r - 0.9817f) * r;
	return ( y < 0.0f ) ? -angle : angle;
}

// http://galba.lunariffic.com/~downl69/blog/?tag=intersection-3-planes
inline bool Intersection3Planes(double *outpt, double *n1, double *p1,double *n2,double *p2, double *n3, double *p3) 
{
	double denom = DotVector(n1, CrossVector(n2, n3).point );
 
    if(fabs(denom) < VTK_DBL_EPSILON )
        return false;

	Add3VectorsWithWeights(outpt, CrossVector(n2, n3).point, CrossVector(n3, n1).point, CrossVector(n1, n2).point, 
								DotVector(n1, p1)/denom, DotVector(n2, p2)/denom, DotVector(n3, p3)/denom);	
    return true;
}
inline double AngleBeteewnTwoVector(PointStruct v, PointStruct u)
{
	double temp = v.Dot(u);
	double angle = 0;
	if (fabs(temp) < 1)
	{
		angle = acos(v.Dot(u))*180.0 / PI;
	}
	else if (fabs(temp - 1)<0.0001)
	{
		angle = 0;
	}
	else
	{
		angle = 180;
	}
	return angle;
}

inline double DistanceOfPointToPlane(PointStruct Pt, PointStruct PonPlane, PointStruct planeNormal)
{
	planeNormal.Unit();

	double d = -planeNormal.Dot(PonPlane);


	return planeNormal.Dot(Pt) + d;
}


inline int MaxInList(std::vector<double> &values, double &MaxValue)
{
	int index = -1;
	MaxValue = -999999;
	for (int i = 0; i < values.size(); i++)
	{
		if (values[i]>MaxValue)
		{
			MaxValue = values[i];
			index = i;
		}
	}

	return index;
}

inline int MinInList(std::vector<double> &values, double &MinValue)
{
	int index = -1;
	MinValue = 999999;
	for (int i = 0; i < values.size(); i++)
	{
		if (values[i]<MinValue)
		{
			MinValue = values[i];
			index = i;
		}
	}

	return index;
}
inline bool CalPlaneLineIntersectPoint(PointStruct planeNormal, PointStruct PointOnPlane, PointStruct lineVector, PointStruct linePoint, PointStruct& intersectPoint)
{
	double vp1, vp2, vp3, n1, n2, n3, v1, v2, v3, m1, m2, m3, t, vpt;
	vp1 = planeNormal[0];
	vp2 = planeNormal[1];
	vp3 = planeNormal[2];
	n1 = PointOnPlane[0];
	n2 = PointOnPlane[1];
	n3 = PointOnPlane[2];
	v1 = lineVector[0];
	v2 = lineVector[1];
	v3 = lineVector[2];
	m1 = linePoint[0];
	m2 = linePoint[1];
	m3 = linePoint[2];
	vpt = v1*vp1 + v2*vp2 + v3*vp3;
	if (vpt == 0)
	{
		return false;
	}
	else
	{
		t = ((n1 - m1)*vp1 + (n2 - m2)*vp2 + (n3 - m3)*vp3) / vpt;
		intersectPoint.SetPoint(m1 + v1*t, m2 + v2*t, m3 + v3*t);
	}
}
