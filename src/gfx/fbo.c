#include "fbo.h"

FBO fbo_create() {
    FBO self;
    glGenFramebuffers(1, &self.handle);
    return self;
}

void fbo_destroy(FBO self) { glDeleteFramebuffers(1, &self.handle); }

void fbo_bind(FBO self) { glBindFramebuffer(GL_FRAMEBUFFER, self.handle); }

void fbo_texture(FBO self, Texture texture) {
    fbo_bind(self);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture.handle,
                         0);
}

void fbo_rbo(FBO self, RBO rbo, GLenum attachment) {
    fbo_bind(self);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbo.handle);
}
