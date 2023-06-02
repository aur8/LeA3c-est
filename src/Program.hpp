#pragma once

#include "p6/p6.h"

struct Program {
  p6::Shader _program;

  // vertex matrix
  GLint uMVPMatrix;
  GLint uMVMatrix;
  GLint uNormalMatrix;

  // textures
  GLint uTexture;

  Program()
      : _program{p6::load_shader("shaders/3D.vs.glsl",
                                 "shaders/pointLight.fs.glsl")} {
    uMVPMatrix = glGetUniformLocation(_program.id(), "uMVPMatrix");
    uMVMatrix = glGetUniformLocation(_program.id(), "uMVMatrix");
    uNormalMatrix = glGetUniformLocation(_program.id(), "uNormalMatrix");

    uTexture = glGetUniformLocation(_program.id(), "uTexture");
  }
};
