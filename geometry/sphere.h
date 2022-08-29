#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "../common/vec3.h"

class sphere : public hittable {
public:
  point3 center;
  double radius;

public:
  sphere() {};
  sphere(point3 _center, double _radius) : center(_center), radius(_radius) {};

  virtual bool hit(const ray& ray, double t_min, double t_max, hit_record& hit) const override;
};

bool sphere::hit(const ray& ray, double t_min, double t_max, hit_record& hit) const {
  /* We are simply using a sphere equation x^2 + y^2 + z^2 = r^2.
    Then expressing each point as (x-C_x)^2 = (P(t)-C_X)^2...
    and taking it as a parametric equation.
    Given P(t) = A - tb for a ray with origin A and direction b.
    Then we solve for t using the general solution for quadratics.
  */

  vec3 dc = ray.origin() - center;
  auto a = ray.direction().length_squared(); //a vector dotted with itself yields its squared lenght

  // b has 2 as a factor from the sphere equation. So we can cancel it here.
  auto half_b = dot(ray.direction(), dc);
  auto c = dc.length_squared() - (radius * radius);

  // Same, as we factored sqrt(4) from the discriminant. and it gets canceled with the denominator.
  auto discriminant = half_b * half_b - a * c;

  if (discriminant < 0) return false;

  auto sqrtd = sqrt(discriminant);
  auto root = (-half_b - sqrtd) / a;

  // check if the t_min < root < t_max for any root.
  if (root < t_min || t_max < root) {
    root = (-half_b + sqrtd) / a;
    if (root < t_min || t_max < root) {
      return false;
    }
  }

  // only fill the hit_record pointer if there was a hit.
  hit.t = root;
  hit.p = ray.at(hit.t);
  vec3 outward_normal = (hit.p - center) / radius;
  hit.set_face_normal(ray, outward_normal);

  return true;
}

#endif