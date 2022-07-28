#pragma once

#include "../util/util.h"
#include "gfx.h"
#include "texture.h"
#include "rbo.h"

typedef struct FBO {
  GLuint handle;
} FBO;

FBO fbo_create();
void fbo_destroy(FBO self);
void fbo_bind(FBO self);
void fbo_texture(FBO self, Texture texture);
void fbo_rbo(FBO self, RBO rbo, GLenum attachment);
