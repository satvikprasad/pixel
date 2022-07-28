#pragma once

#include "../util/util.h"

typedef struct PointLight {
    vec3s ambient;
    vec3s diffuse;
    vec3s specular;
    vec3s position;
    f32 constant;
    f32 linear;
    f32 quadratic;
} PointLight;

typedef struct DirLight {
    vec3s direction;
  
    vec3s ambient;
    vec3s diffuse;
    vec3s specular;
} DirLight;  
