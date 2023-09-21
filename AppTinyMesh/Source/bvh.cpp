#include <vector>

#include <cmath>

#include "HitInfo.h"
#include "bvh.h"
#include "vec.h"

Vector BVH::BoundingVolume::PLANE_NORMALS[BVH::BoundingVolume::PLANES_COUNT] = {
    Vector(1, 0, 0),
    Vector(0, 1, 0),
    Vector(0, 0, 1),
    Vector(std::sqrt(3.0f) / 3, std::sqrt(3.0f) / 3, std::sqrt(3.0f) / 3),
    Vector(-std::sqrt(3.0f) / 3, std::sqrt(3.0f) / 3, std::sqrt(3.0f) / 3),
    Vector(-std::sqrt(3.0f) / 3, -std::sqrt(3.0f) / 3, std::sqrt(3.0f) / 3),
    Vector(std::sqrt(3.0f) / 3, -std::sqrt(3.0f) / 3, std::sqrt(3.0f) / 3),
};

BVH::BVH() : _root(nullptr), _triangles(nullptr) {}

BVH::BVH(const BVH& other)
{
    _triangles = other._triangles;
    _root = other._root;
}

BVH::BVH(BVH&& other)
{
    _triangles = other._triangles;
    _root = other._root;

    other._triangles = nullptr;
    other._root = nullptr;
}

BVH::BVH(std::vector<Triangle>* triangles, int max_depth, int leaf_max_obj_count) : _triangles(triangles)
{
    BoundingVolume volume;
    Point minimum(INFINITY, INFINITY, INFINITY), maximum(-INFINITY, -INFINITY, -INFINITY);

    for (const Triangle& triangle : *triangles)
    {
        volume.extend_volume(triangle);

        for (int i = 0; i < 3; i++)
        {
            minimum = min(minimum, Point(triangle[i].x, triangle[i].y, triangle[i].z));
            maximum = max(maximum, Point(triangle[i].x, triangle[i].y, triangle[i].z));
        }
    }

    //We now have a bounding volume to work with
    build_bvh(max_depth, leaf_max_obj_count, minimum, maximum, volume);
}

BVH::~BVH()
{
    delete _root;
}

void BVH::operator=(const BVH& other)
{
    _triangles = other._triangles;
    _root = other._root;
}

void BVH::operator=(BVH&& other)
{
    _triangles = other._triangles;
    _root = other._root;

    other._root = nullptr;
}

void BVH::build_bvh(int max_depth, int leaf_max_obj_count, Point min, Point max, const BoundingVolume& volume)
{
    _root = new OctreeNode(min, max);

    for (Triangle& triangle : *_triangles)
        _root->insert(&triangle, 0, max_depth, leaf_max_obj_count);

    _root->compute_volume();
}

bool BVH::intersect(const Ray& ray, HitInfo& hit_info) const
{
    return _root->intersect(ray, hit_info);
}

