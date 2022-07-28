/* date = June 30th 2022 4:11 pm */

#ifndef CAMERA_H
#define CAMERA_H

#include "util.h"

#include <cglm/cglm.h>
#include <cglm/struct.h>

typedef enum CameraType
{
    CameraType_perspective, CameraType_ortho, CameraType_MAX
} CameraType;


typedef struct ViewProj
{
    mat4s view, proj;
} ViewProj;

typedef struct OrthoCamera
{
    struct ViewProj view_proj;
    vec2s position, min, max;
} OrthoCamera;

typedef struct PerspectiveCamera
{
    ViewProj view_proj;
    vec3s position, direction, up, right;
    f32 pitch, yaw, fov, aspect, znear, zfar;
} PerspectiveCamera;

void perspective_camera_init(PerspectiveCamera *self, f32 fov);
void perspective_camera_update(PerspectiveCamera *self);

void ortho_camera_init(OrthoCamera *self, vec2s min, vec2s max);
void ortho_camera_update(OrthoCamera *self);

#endif //CAMERA_H
