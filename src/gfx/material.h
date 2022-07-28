#pragma once

#include "gfx.h"
#include "../util/util.h"

typedef struct Material {
    vec3s diffuse;
    vec3s specular;
    f32 shine;
} Material;

