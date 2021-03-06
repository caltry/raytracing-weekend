#pragma once

#include "hittable.h"

class hittable_list: public hittable {
public:
    hittable_list() {}
    hittable_list(hittable **l, int n) {mList = l; mListSize = n;}
    virtual bool hit(const ray<float> &r, float t_min, float t_max,
                     hit_record &rec) const;
private:
    hittable **mList;
    int mListSize;
};

bool hittable_list::hit(const ray<float> &r, float t_min, float t_max,
                   hit_record &rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    float closest_so_far = t_max;
    for(int i = 0; i < mListSize; i++) {
        if(mList[i]->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}
