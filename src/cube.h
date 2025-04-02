#ifndef CUBE_H
#define CUBE_H

#include "hittable.h"
#include "rtcubo.h"
#include <algorithm>
#include <cmath>
#include <limits>

class cube : public hittable
{
public:
    // Construct a cube with a given center and side length.
    cube(const point3 &center, double side)
        : center(center), half_side(side / 2.0) {}

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override
    {
        // Compute the minimum and maximum corners of the cube.
        point3 min_point = center - vec3(half_side, half_side, half_side);
        point3 max_point = center + vec3(half_side, half_side, half_side);

        double tmin, tmax, tymin, tymax, tzmin, tzmax;
        const double small = 1e-8; // tolerance for near-zero direction

        // X-axis slab.
        if (std::abs(r.direction().x()) < small)
        {
            if (r.origin().x() < min_point.x() || r.origin().x() > max_point.x())
                return false;
            tmin = -infinity;
            tmax = infinity;
        }
        else
        {
            tmin = (min_point.x() - r.origin().x()) / r.direction().x();
            tmax = (max_point.x() - r.origin().x()) / r.direction().x();
            if (tmin > tmax)
                std::swap(tmin, tmax);
        }

        // Y-axis slab.
        if (std::abs(r.direction().y()) < small)
        {
            if (r.origin().y() < min_point.y() || r.origin().y() > max_point.y())
                return false;
            tymin = -infinity;
            tymax = infinity;
        }
        else
        {
            tymin = (min_point.y() - r.origin().y()) / r.direction().y();
            tymax = (max_point.y() - r.origin().y()) / r.direction().y();
            if (tymin > tymax)
                std::swap(tymin, tymax);
        }

        if ((tmin > tymax) || (tymin > tmax))
            return false;
        tmin = std::max(tmin, tymin);
        tmax = std::min(tmax, tymax);

        // Z-axis slab.
        if (std::abs(r.direction().z()) < small)
        {
            if (r.origin().z() < min_point.z() || r.origin().z() > max_point.z())
                return false;
            tzmin = -infinity;
            tzmax = infinity;
        }
        else
        {
            tzmin = (min_point.z() - r.origin().z()) / r.direction().z();
            tzmax = (max_point.z() - r.origin().z()) / r.direction().z();
            if (tzmin > tzmax)
                std::swap(tzmin, tzmax);
        }

        if ((tmin > tzmax) || (tzmin > tmax))
            return false;
        tmin = std::max(tmin, tzmin);
        tmax = std::min(tmax, tzmax);

        // Determine the hit time.
        double t_hit = (tmin >= 0) ? tmin : tmax;
        if (!ray_t.surrounds(t_hit))
            return false;

        rec.t = t_hit;
        rec.p = r.at(rec.t);

        // Compute the face normal based on the direction from the cube's center.
        vec3 d = rec.p - center;
        double absX = std::abs(d.x());
        double absY = std::abs(d.y());
        double absZ = std::abs(d.z());
        vec3 outward_normal(0, 0, 0);
        if (absX >= absY && absX >= absZ)
            outward_normal = vec3((d.x() > 0) ? 1 : -1, 0, 0);
        else if (absY >= absX && absY >= absZ)
            outward_normal = vec3(0, (d.y() > 0) ? 1 : -1, 0);
        else
            outward_normal = vec3(0, 0, (d.z() > 0) ? 1 : -1);

        rec.set_face_normal(r, outward_normal);
        return true;
    }

private:
    point3 center;
    double half_side;
};

#endif
