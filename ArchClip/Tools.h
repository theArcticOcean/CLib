#pragma once

#include <iostream>
#include <math.h>

using namespace std;

class PointStruct
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

    friend PointStruct operator*(double v,PointStruct &p) { return p*v; }
    friend PointStruct operator/( PointStruct &p, double v) { return p*(1./v); }
    friend PointStruct operator+(const PointStruct &p1, const PointStruct &p2) { return PointStruct(p1[0]+p2[0],p1[1]+p2[1],p1[2]+p2[2]); }
    friend PointStruct operator-(const PointStruct &p1, const PointStruct &p2) { return PointStruct(p1[0]-p2[0],p1[1]-p2[1],p1[2]-p2[2]); }
    friend double operator*( PointStruct &p1,  PointStruct &p2) { return p1[0]*p2[0]+p1[1]*p2[1]+p1[2]*p2[2]; }
    friend PointStruct operator^(const PointStruct &p1, const PointStruct &p2) { return PointStruct(p1[1]*p2[2]-p1[2]*p2[1],p1[2]*p2[0]-p1[0]*p2[2],p1[0]*p2[1]-p1[1]*p2[0]); }
    friend bool operator==( PointStruct &p1,  PointStruct &p2) { return ((p1[0]==p2[0]) && (p1[1]==p2[1]) && (p1[2]==p2[2])); }
    friend bool operator!=( PointStruct &p1,  PointStruct &p2) { return ((p1[0]!=p2[0]) || (p1[1]!=p2[1]) || (p1[2]!=p2[2])); }
    friend bool operator<( PointStruct &p1,  PointStruct &p2) { return (p1[0] <=  p2[0] && p1[1] <= p2[1] && p1[2] <= p2[2]); }
    friend ostream& operator<<(ostream & os, PointStruct & p) { os << "PointStruct [" << p[0] <<", " << p[1] <<", " << p[2] << "]\n"; return os; }
};
