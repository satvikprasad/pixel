/* date = July 1st 2022 9:56 am */

#ifndef AABB_H
#define AABB_H

#include <cglm/cglm.h>
#include <cglm/struct.h>

typedef vec3s AABB[2];

static inline void glms_aabb_scale(AABB box, vec3s scale, AABB dest)
{
    vec3s center = glms_aabb_center(box);
    vec3s size = glms_vec3_sub(box[1], box[0]);
    
    vec3s new_size = glms_vec3_mul(size, scale);
    vec3s half_new_size = glms_vec3_scale(new_size, 0.5f);
    
    // scaled AABB centered around same center
    dest[0] = glms_vec3_add(glms_vec3_scale(half_new_size, -1.0f), center);
    dest[1] = glms_vec3_add(glms_vec3_sub(new_size, half_new_size), center);
}

#endif //AABB_H
