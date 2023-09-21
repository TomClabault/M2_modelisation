#pragma once

#include "bvh.h"
#include "box.h"
#include "ray.h"
#include "mathematics.h"

//! Compute the barycenter of the triangle.
inline Vector Triangle::Center() const
{
    return (p[0] + p[1] + p[2]) / 3.0;
}

//! Compute the area of the triangle.
inline double Triangle::Area() const
{
    return 0.5 * Norm((p[0] - p[1]) / (p[2] - p[0]));
}

/*!
\brief Create a triangle.
\param a,b,c Vertices of the triangle.
*/
inline Triangle::Triangle(const Vector& a, const Vector& b, const Vector& c)
{
    p[0] = a;
    p[1] = b;
    p[2] = c;

    _face_normal = (b-a) / (c-a);
}


class QString;

class Mesh
{
protected:
    std::vector<Vector> m_vertices; //!< Vertices.
    std::vector<Vector> m_normals;  //!< Normals.
    std::vector<int> m_varray;     //!< Vertex indexes.
    std::vector<int> m_narray;     //!< Normal indexes.

    //Dynamically allocated triangles for the BVH
    std::vector<Triangle>* m_triangles;
    BVH* m_bvh;
public:
    explicit Mesh();
    explicit Mesh(const std::vector<Vector>&, const std::vector<int>&);
    explicit Mesh(const std::vector<Vector>& vertices, const std::vector<Vector>& normals, const std::vector<int>& va, const std::vector<int>& na);
    ~Mesh();

    void Reserve(int, int, int, int);

    Triangle GetTriangle(int) const;
    Vector Vertex(int) const;
    Vector Vertex(int, int) const;

    Vector Normal(int) const;

    int Triangles() const;
    int Vertexes() const;

    std::vector<int> VertexIndexes() const;
    std::vector<int> NormalIndexes() const;

    int VertexIndex(int, int) const;
    int NormalIndex(int, int) const;

    Vector intersect(const Ray& ray) const;

    Vector operator[](int) const;

    Box GetBox() const;

    void Scale(double);

    void SmoothNormals();

    // Constructors from core classes
    explicit Mesh(const Box&);

    void Load(const QString&);
    void SaveObj(const QString&, const QString&) const;
protected:
    void AddTriangle(int, int, int, int);
    void AddSmoothTriangle(int, int, int, int, int, int);
    void AddSmoothQuadrangle(int, int, int, int, int, int, int, int);
    void AddQuadrangle(int, int, int, int);
};

/*!
\brief Return the set of vertex indexes.
*/
inline std::vector<int> Mesh::VertexIndexes() const
{
    return m_varray;
}

/*!
\brief Return the set of normal indexes.
*/
inline std::vector<int> Mesh::NormalIndexes() const
{
    return m_narray;
}

/*!
\brief Get the vertex index of a given triangle.
\param t Triangle index.
\param i Vertex index.
*/
inline int Mesh::VertexIndex(int t, int i) const
{
    return m_varray.at(t * 3 + i);
}

/*!
\brief Get the normal index of a given triangle.
\param t Triangle index.
\param i Normal index.
*/
inline int Mesh::NormalIndex(int t, int i) const
{
    return m_narray.at(t * 3 + i);
}

/*!
\brief Get a triangle.
\param i Index.
\return The triangle.
*/
inline Triangle Mesh::GetTriangle(int i) const
{
    return Triangle(m_vertices.at(m_varray.at(i * 3 + 0)), m_vertices.at(m_varray.at(i * 3 + 1)), m_vertices.at(m_varray.at(i * 3 + 2)));
}

/*!
\brief Get a vertex.
\param i The index of the wanted vertex.
\return The wanted vertex (as a 3D Vector).
*/
inline Vector Mesh::Vertex(int i) const
{
    return m_vertices[i];
}

/*!
\brief Get a vertex from a specific triangle.
\param t The number of the triangle wich contain the wanted vertex.
\param v The triangle vertex: 0, 1, or 2.
\return The wanted vertex (as a 3D Vector).
*/
inline Vector Mesh::Vertex(int t, int v) const
{
    return m_vertices[m_varray[t * 3 + v]];
}

/*!
\brief Get the number of vertices in the geometry.
\return The number of vertices in the geometry, in other words the size of vertices.
*/
inline int Mesh::Vertexes() const
{
    return int(m_vertices.size());
}

/*!
\brief Get a normal.
\param i Index of the wanted normal.
\return The normal.
*/
inline Vector Mesh::Normal(int i) const
{
    return m_normals[i];
}

/*!
\brief Get the number of triangles.
*/
inline int Mesh::Triangles() const
{
    return int(m_varray.size()) / 3;
}

/*!
\brief Get a vertex.
\param i The index of the wanted vertex.
\return The wanted vertex (as a 3D Vector).
\see vertex(int i) const
*/
inline Vector Mesh::operator[](int i) const
{
    return m_vertices[i];
}

