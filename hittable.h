//
// Created by Jorge on 12/17/2024.
//

#ifndef HITTABLE_H
#define HITTABLE_H
#include "rtweekend.h"

class hit_record {
public:
    point3 p; // the (world) position where the hit happened
    vec3 normal; // the normal direction of the hit, along the hitted surface
    double t; // how far along the ray did the hit happen
    bool front_face; // has the ray hit on the front or back sides of the face?
    void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual ~hittable() = default;
    virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const = 0;
};


#endif //HITTABLE_H
