#pragma once
#include "Program.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

struct Point_light {

  glm::vec3 m_position;
  glm::vec3 m_intensity;

  GLint uLightPosition1;
  GLint uLightIntensity1;
  GLint uLightPosition2;
  GLint uLightIntensity2;

  Point_light(glm::vec3 position, glm::vec3 intensity, p6::Shader &program)
      : m_position(position), m_intensity(intensity) {
    uLightPosition1 = glGetUniformLocation(program.id(), "uLightPos1");
    uLightIntensity1 = glGetUniformLocation(program.id(), "uLightIntensity1");
    uLightPosition2 = glGetUniformLocation(program.id(), "uLightPos2");
    uLightIntensity2 = glGetUniformLocation(program.id(), "uLightIntensity2");
  }
};

struct Dir_light {

  glm::vec3 m_direction;
  glm::vec3 m_intensity;

  GLint uLightDir;
  GLint uLightIntensity3;

  Dir_light(glm::vec3 direction, glm::vec3 intensity, p6::Shader &program)
      : m_direction(direction), m_intensity(intensity) {
    uLightDir = glGetUniformLocation(program.id(), "uLightDir");
    uLightIntensity3 = glGetUniformLocation(program.id(), "uLightIntensity3");
  }
};

void draw_point_light(Point_light light, glm::mat4 viewMatrix,
                      int index_light) {

  glm::vec3 lightViewPosition =
      glm::vec3(viewMatrix * glm::vec4(light.m_position, 1.f));

  if (index_light == 1) {

    glUniform3fv(light.uLightPosition1, 1, glm::value_ptr(lightViewPosition));

    glUniform3fv(light.uLightIntensity1, 1, glm::value_ptr(light.m_intensity));
  } else {
    glUniform3fv(light.uLightPosition2, 1, glm::value_ptr(lightViewPosition));

    glUniform3fv(light.uLightIntensity2, 1, glm::value_ptr(light.m_intensity));
  }
}

void draw_dir_light(Dir_light light, glm::mat4 viewMatrix) {

  glm::vec3 lightViewDir =
      glm::vec3(viewMatrix * glm::vec4(light.m_direction, 1.f));
  glUniform3fv(light.uLightDir, 1, glm::value_ptr(lightViewDir));
  glUniform3fv(light.uLightIntensity3, 1, glm::value_ptr(light.m_intensity));
}
