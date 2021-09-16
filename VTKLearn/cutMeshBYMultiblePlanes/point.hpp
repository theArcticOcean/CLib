#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>
#include <math.h>
using namespace std;

class Point
{
public:
    double point[3];
    Point() {}
    Point(double x, double y, double z) { point[0]=x; point[1]=y; point[2]=z;}
    Point(double *ptr) { point[0]=ptr[0]; point[1]=ptr[1]; point[2]=ptr[2];}
    Point & SetPoint(double p[]) { point[0]=p[0]; point[1]=p[1];  point[2]=p[2];  return *this; }
    double operator[](int i) const { return point[i]; }
    double &operator[](int i)  { return point[i]; }

    Point operator+(double v) { return Point(point[0]+v, point[1]+v, point[2]+v); }
    Point operator-(double v) { return Point(point[0]-v, point[1]-v, point[2]-v); }
    Point operator*(double v) { return Point(point[0]*v, point[1]*v, point[2]*v); }
    Point operator/(double v) { return Point(point[0]/v, point[1]/v, point[2]/v); }
    Point operator-() { return Point(-point[0], -point[1], -point[2]); }
    Point operator/=(double v) { point[0]/=v; point[1]/=v; point[2]/=v;   return *this; }
    double Dot(const Point &p) { return point[0]*p.point[0]+point[1]*p.point[1]+point[2]*p.point[2]; }
    double Length() { return sqrt(point[0]*point[0]+point[1]*point[1]+point[2]*point[2]); }
    Point Unit() { return (operator/=(Length())); }

    friend Point operator*(double v,Point &p) { return p*v; };
    friend Point operator/( Point &p, double v) { return p*(1./v); }
    friend Point operator+(const Point &p1, const Point &p2) { return Point(p1[0]+p2[0],p1[1]+p2[1],p1[2]+p2[2]); }
    friend Point operator-(const Point &p1, const Point &p2) { return Point(p1[0]-p2[0],p1[1]-p2[1],p1[2]-p2[2]); }
    friend Point operator^(const Point &p1, const Point &p2) { return Point(p1[1]*p2[2]-p1[2]*p2[1],p1[2]*p2[0]-p1[0]*p2[2],p1[0]*p2[1]-p1[1]*p2[0]); }
    friend bool operator==( Point &p1,  Point &p2)
    {
        bool notEqual = (p1 != p2);
        return !notEqual;
    }
    friend bool operator!=( Point &p1,  Point &p2)
    {
        if( fabs( p1[0] - p2[0] ) > 1e-6 ||
            fabs( p1[1] - p2[1] ) > 1e-6 ||
            fabs( p1[2] - p2[2] ) > 1e-6 )
        {
            return true;
        }
        return false;
    }

    bool operator<( const Point &p2 ) const
    {
        return ( point[0] <=  p2[0] && point[1] <= p2[1] && point[2] <= p2[2] );
    }
};

#endif //POINT_HPP
