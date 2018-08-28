#ifndef POINT_STRUCT_HPP
#define POINT_STRUCT_HPP

struct PointStruct
{
    double point[3];
    PointStruct( double *data )
    {
        point[0] = data[0];
        point[1] = data[1];
        point[2] = data[2];
    }
    PointStruct( double data0, double data1, double data2 )
    {
        point[0] = data0;
        point[1] = data1;
        point[2] = data2;
    }
    double operator [](int index)
    {
        return point[index];
    }
    PointStruct operator -( PointStruct other)
    {
        return PointStruct(
                point[0] - other[0],
                point[1] - other[1],
                point[2] - other[2]
                );
    }
    PointStruct operator +( PointStruct other)
    {
        return PointStruct(
                point[0] + other[0],
                point[1] + other[1],
                point[2] + other[2]
                );
    }
    PointStruct dot( PointStruct other )
    {
        return PointStruct(
                point[1]*other[2] - other[1]*point[2],
                other[1]*point[2] - point[0]*other[2],
                point[0]*other[1] - other[0]*point[1]
                );
    }
};

#endif
