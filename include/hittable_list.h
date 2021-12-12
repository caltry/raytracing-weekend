#pragma once

#include <list>

#include "hittable.h"

class hittable_list: public hittable {
public:
    hittable_list() {}
    hittable_list(hittable **l, int n)
    {
        for(unsigned i = 0; i < n; i++)
        {
            mList.push_back(l[i]);
        }
    }
    hittable_list(const std::list<hittable*> &l)
    : mList(l) {}
    virtual bool hit(const ray<float> &r, float t_min, float t_max,
                     hit_record &rec) const;
private:
    std::list<hittable*> mList;
    int mListSize;
};

bool hittable_list::hit(const ray<float> &r, float t_min, float t_max,
                   hit_record &rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    float closest_so_far = t_max;
    for(auto it = mList.begin(); it != mList.end(); it++) {
        if((*it)->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}
