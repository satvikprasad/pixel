#pragma once

#include "gfx.h"
#include "../util/util.h"

typedef struct RBO {
  GLuint handle;
} RBO;

RBO rbo_create(GLenum type, vec2s size);
void rbo_destroy(RBO self);
void rbo_bind(RBO self);
