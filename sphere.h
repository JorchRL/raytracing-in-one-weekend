//
// Created by Jorge on 12/17/2024.
//

#ifndef SPHERE_H
#define SPHERE_H
#include "hittable.h"

class sphere : public hittable {
public:
    sphere(const point3& center, double radius) :
        center(center),
        radius(std::fmax(0, radius))
    {}
    bool hit(const ray &r, interval ray_t, hit_record &rec) const override{
        vec3 oc = center - r.origin(); // the line from the origin of the ray to the center of the sphere
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius * radius;
        auto discriminant = h * h - a * c;
        if (discriminant < 0) {
            return false;
        }

        auto sqrtd = std::sqrt(discriminant);

        auto root = (h - sqrtd) / a;
        if (! ray_t.surrounds(root) ) {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root)) {
                return false;
            }
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        rec.normal = (rec.p - center) / radius;

        // for the case of a sphere we can hack the unit normal of its surface by dividing the
        // normal vector by its radius.
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);

        return true;
    }

private:
    point3 center;
    double radius;
};

#endif //SPHERE_H
