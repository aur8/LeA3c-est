#pragma once

#include "p6/p6.h"

struct Program {
  p6::Shader _program;

  // vertex matrix
  GLint uMVPMatrix;
  GLint uMVMatrix;
  GLint uNormalMatrix;

  // light parameters
  GLint uLightPosition;
  GLint uLightIntensity;
  GLint uLightPosition2;
  GLint uLightIntensity2;
  GLint uLightDir;
  GLint uLightIntensity3;

  // object materials
  GLint uKd;
  GLint uKs;
  GLint uShininess;

  // textures
  GLint uTexture;

  Program()
      : _program{p6::load_shader("shaders/3D.vs.glsl",
                                 "shaders/pointLight.fs.glsl")} {
    uMVPMatrix = glGetUniformLocation(_program.id(), "uMVPMatrix");
    uMVMatrix = glGetUniformLocation(_program.id(), "uMVMatrix");
    uNormalMatrix = glGetUniformLocation(_program.id(), "uNormalMatrix");

    uLightPosition = glGetUniformLocation(_program.id(), "uLightPos1");
    uLightIntensity = glGetUniformLocation(_program.id(), "uLightIntensity1");
    uLightPosition2 = glGetUniformLocation(_program.id(), "uLightPos2");
    uLightIntensity2 = glGetUniformLocation(_program.id(), "uLightIntensity2");
    uLightDir = glGetUniformLocation(_program.id(), "uLightDir");
    uLightIntensity3 = glGetUniformLocation(_program.id(), "uLightIntensity3");

    uKd = glGetUniformLocation(_program.id(), "uKd");
    uKs = glGetUniformLocation(_program.id(), "uKs");
    uShininess = glGetUniformLocation(_program.id(), "uShininess");

    uTexture = glGetUniformLocation(_program.id(), "uTexture");
  }
};
