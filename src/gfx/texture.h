#ifndef TEXTURE_H
#define TEXTURE_H

#include "../util/util.h"
#include "gfx.h"

typedef struct Texture {
  GLuint handle;
  ivec2s size;
  vec2s uv_unit;
} Texture;

void texture_load_pixels(char *path, u8 **pixels_out, u64 *width_out,
                         u64 *height_out);
Texture texture_create_from_pixels(u8 *pixels, u64 width, u64 height);
Texture texture_create_from_path(char *path);
Texture texture_create_empty(u64 width, u64 height);
void texture_destroy(Texture self);
void texture_bind(Texture self);

#endif
