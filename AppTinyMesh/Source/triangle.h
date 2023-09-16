#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "AppTinyMesh/Source/gKit/vec.h"
#include "HitInfo.h"
#include "box.h"
#include "ray.h"

// Triangle
class Triangle
{
protected:
    Vector p[3] = {Vector(0.0,0.0,0.0),Vector(1.0,0.0,0.0), Vector(0.0,1.0,0.0), }; //!< Array of vertices.

    Vector _face_normal;
public:
    //! Empty.
    Triangle() {}
    explicit Triangle(const Vector&, const Vector&, const Vector&);

    //! Empty.
    ~Triangle() {}

    // Point in triangle
    Vector Vertex(double, double) const;

    bool barycentric_coordinates(const Point& point, float& u, float& v) const
    {
        Vector point_v = Vector(point.x, point.y, point.z);

        Vector ab = p[1] - p[0];
        Vector ac = p[2] - p[0];

        float triangleArea = length(cross(ab, ac));

        Vector ca = p[0] - p[2];
        Vector cp = point_v - p[2];
        Vector crossVec = cross(ca, cp);
        if (dot(_face_normal, crossVec) < 0)
            return false;
        u = length(crossVec) / triangleArea;

        Vector ap = point_v - p[0];
        crossVec = cross(ab, ap);
        if (dot(_face_normal, crossVec) < 0)
            return false;
        v = length(crossVec) / triangleArea;

        Vector bc = p[2] - p[1];
        Vector bp = point_v - p[1];
        crossVec = cross(bc, bp);
        if (dot(_face_normal, crossVec) < 0)
            return false;

        return true;
    }

    // Intersection
    bool Intersect(const Ray&, double&, double&, double&) const;
    bool intersect(const Ray& ray, HitInfo& hitInfo) const
    {
        //Common variables to both algorithms
        float t, u, v;

        Vector ab = p[1] - p[0];
        Vector ac = p[2] - p[0];
        Vector OA = ray._origin - p[0];
        Vector minusDcrossOA = cross(-ray._direction, OA);

        float Mdet = dot(_face_normal, -ray._direction);
        if (Mdet == 0)//If == 0, ray parallel to triangle
            return false;

        Mdet = 1 / Mdet;//Inverting the determinant once and for all

        //Cramer's rule
        u = dot(minusDcrossOA, ac) * Mdet;
        if (u < 0 || u > 1)
            return false;

        v = dot(minusDcrossOA, -ab) * Mdet;
        if (v < 0 || u + v > 1)
            return false;

        //Intersection point in the triangle at this point, computing t
        t = dot(_face_normal, OA) * Mdet;
        if (t < 0)
            return false;


        //Common operations to both algorithms
        hitInfo.t = t;
        //Barycentric coordinates are: P = (1 - u - v)A + uB + vC
        hitInfo.u = u;
        hitInfo.v = v;
        hitInfo.triangle = this;

        return true;
    }

    void Translate(const Vector&);

    // Geometry
    Vector Normal() const;
    Vector AreaNormal() const;
    Vector Center() const;

    double Area() const;
    double Aspect() const;
    Box GetBox() const;

    Point bbox_centroid() const
    {
        Vector mini = (min(p[0], min(p[1], p[2])) + max(p[0], max(p[1], p[2]))) / 2;
        return Point(mini.x, mini.y, mini.z);
    }

    Vector& operator[] (int i)
    {
        return (*((&p[0]) + i));
    }

    const Vector& operator[] (int i) const
    {
        return (*((&p[0]) + i));
    }

    // Stream
    friend std::ostream& operator<<(std::ostream&, const Triangle&);

    double InscribedRadius() const;
    double CircumscribedRadius() const;
protected:
    static double epsilon; //!< Internal epsilon constant.
};

#endif // TRIANGLE_H
