#ifndef CUBE_H
#define CUBE_H

#include "hittable.h"
#include "rtcubo.h"
#include "vec3.h"
#include <algorithm>
#include <cmath>

inline vec3 rotate_y(const vec3 &v, double angle)
{
    double cos_theta = std::cos(angle);
    double sin_theta = std::sin(angle);
    return vec3(cos_theta * v.x() + sin_theta * v.z(),
                v.y(),
                -sin_theta * v.x() + cos_theta * v.z());
}

class cube : public hittable
{
public:
    cube(const point3 &center, double side_length, shared_ptr<material> mat, double rotation_degrees = 45.0)
        : center(center), side_length(side_length), mat(mat)
    {
        rotation = degrees_to_radians(rotation_degrees);
        half = side_length / 2.0;
    }

    virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const override
    {
        vec3 origin_local = rotate_y(r.origin() - center, -rotation);
        vec3 direction_local = rotate_y(r.direction(), -rotation);
        ray r_local(origin_local, direction_local);

        double t_min = ray_t.min;
        double t_max = ray_t.max;
        for (int i = 0; i < 3; i++)
        {
            double invD = 1.0 / r_local.direction().e[i];
            double t0 = (-half - r_local.origin().e[i]) * invD;
            double t1 = (half - r_local.origin().e[i]) * invD;
            if (invD < 0.0)
                std::swap(t0, t1);
            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;
            if (t_max <= t_min)
                return false;
        }
        double t_hit = t_min;

        point3 hit_local = r_local.at(t_hit);

        double dx = half - std::fabs(hit_local.x());
        double dy = half - std::fabs(hit_local.y());
        double dz = half - std::fabs(hit_local.z());
        double min_delta = std::min({dx, dy, dz});
        vec3 local_normal;
        if (min_delta == dx)
            local_normal = (hit_local.x() > 0) ? vec3(1, 0, 0) : vec3(-1, 0, 0);
        else if (min_delta == dy)
            local_normal = (hit_local.y() > 0) ? vec3(0, 1, 0) : vec3(0, -1, 0);
        else
            local_normal = (hit_local.z() > 0) ? vec3(0, 0, 1) : vec3(0, 0, -1);

        point3 hit_point = rotate_y(hit_local, rotation) + center;
        vec3 world_normal = rotate_y(local_normal, rotation);

        rec.t = t_hit;
        rec.p = hit_point;
        rec.mat = mat;
        rec.set_face_normal(r, world_normal);

        return true;
    }

private:
    point3 center;
    double side_length;
    double half;
    double rotation;
    shared_ptr<material> mat;
};

#endif
