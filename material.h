//
// Created by Jorge on 12/21/2024.
//

#ifndef MATERIAL_H
#define MATERIAL_H
#include "hittable.h"
#include "ray.h"
#include "vec3.h"

class material
{
    public:
    virtual ~material() = default;

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    {
        return false;
    }
};

#endif //MATERIAL_H
