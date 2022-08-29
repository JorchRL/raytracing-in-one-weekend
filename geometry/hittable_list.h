#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {

public:
  std::vector<shared_ptr<hittable>> objects;

public:
  hittable_list() { objects.clear(); }
  hittable_list(shared_ptr<hittable> object) {
    add(object);
  }

  void clear() {
    objects.clear();
  }

  void add(shared_ptr<hittable> object) {
    objects.push_back(object);
  }

  virtual bool hit(const ray& ray, double t_min, double t_max, hit_record& hit) const override;
};


bool hittable_list::hit(const ray& ray, double t_min, double t_max, hit_record& hit) const {
  hit_record temp_rec;
  bool hit_anything = false;
  auto closest_so_far = t_max;

  // go through all the objects and see if any is hit.
  for (const auto& object : objects) {
    if (object->hit(ray, t_min, t_max, temp_rec)) {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      hit = temp_rec;
    }
  }

  return hit_anything;
}

#endif