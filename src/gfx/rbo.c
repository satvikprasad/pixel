#include "rbo.h"

RBO rbo_create(GLenum type, vec2s size) {
    RBO self;
    glGenRenderbuffers(1, &self.handle);
    
    rbo_bind(self);
    glRenderbufferStorage(GL_RENDERBUFFER, type, size.x, size.y);
    return self;
}

void rbo_destroy(RBO self) {
    glDeleteRenderbuffers(1, &self.handle); 
}

void rbo_bind(RBO self) {
    glBindRenderbuffer(GL_RENDERBUFFER, self.handle);
}


