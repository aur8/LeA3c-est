#pragma once

#include "glimac/common.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"
#include "tiny_obj_loader.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class Model {
public:
  Model(const std::string &filename) { Load_model(filename); }

  void Load_model(const std::string &filename);

  const std::vector<glimac::ShapeVertex> &getVertices() const {
    return m_vertices;
  }

  const tinyobj::attrib_t &getAttrib() const { return m_attrib; }

  const std::vector<tinyobj::shape_t> &getShapes() const { return m_shapes; }

  const std::vector<tinyobj::material_t> &getMaterials() const {
    return m_materials;
  }

  Model() = default;

  ~Model() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
  };

  void create_vbo();
  void create_vao();
  GLuint get_vbo() { return m_vbo; };
  GLuint get_vao() { return m_vao; };

private:
  GLuint m_vbo;
  GLuint m_vao;
  tinyobj::attrib_t m_attrib;
  std::vector<tinyobj::shape_t> m_shapes;
  std::vector<tinyobj::material_t> m_materials;
  std::vector<glimac::ShapeVertex> m_vertices;
};
