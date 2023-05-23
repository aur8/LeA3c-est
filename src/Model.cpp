#include "Model.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

void Model::Load_model(const std::string &filename) {
  std::string err;
  std::string warn;

  std::string objFile = "assets/models/" + filename;
  // std::string mtlFile =
  // "/home/pauline/Documents/S4/Papeterie/assets/models/";

  bool ret = tinyobj::LoadObj(&m_attrib, &m_shapes, &m_materials, &warn, &err,
                              objFile.c_str());

  if (!warn.empty()) {
    std::cerr << warn << std::endl;
  }

  if (!err.empty()) {
    throw std::runtime_error(err);
  }

  if (!ret) {
    throw std::runtime_error("Failed to load OBJ file");
  }

  for (const auto &shape : m_shapes) {
    size_t index_offset = 0;
    for (size_t i = 0; i < shape.mesh.num_face_vertices.size(); i++) {
      size_t fv = shape.mesh.num_face_vertices[i];

      for (size_t v = 0; v < fv; v++) {
        tinyobj::index_t index = shape.mesh.indices[index_offset + v];

        glimac::ShapeVertex vertex;

        vertex.position =
            glm::vec3(m_attrib.vertices[3 * index.vertex_index + 0],
                      m_attrib.vertices[3 * index.vertex_index + 1],
                      m_attrib.vertices[3 * index.vertex_index + 2]);

        vertex.normal = glm::vec3(m_attrib.normals[3 * index.normal_index + 0],
                                  m_attrib.normals[3 * index.normal_index + 1],
                                  m_attrib.normals[3 * index.normal_index + 2]);

        vertex.texCoords =
            glm::vec2(m_attrib.texcoords[2 * index.texcoord_index + 0],
                      m_attrib.texcoords[2 * index.texcoord_index + 1]);

        m_vertices.push_back(vertex);
      }
      index_offset += fv;
    }
  }
}

void Model::create_vbo() {
  // creating & binding vbo
  m_vbo = 0;
  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

  // creating vertices
  const std::vector<glimac::ShapeVertex> model_vertices = m_vertices;

  // sending data to GPU
  glBufferData(GL_ARRAY_BUFFER,
               model_vertices.size() * sizeof(glimac::ShapeVertex),
               model_vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::create_vao() {
  // creating & binding vao
  m_vao = 0;
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  // binding vbo
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

  const GLuint VERTEX_ATTR_POSITION = 0;
  const GLuint VERTEX_ATTR_NORMAL = 1;
  const GLuint VERTEX_ATTR_TEXCOORDS = 2;

  glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
  glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
  glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

  glVertexAttribPointer(
      VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex),
      (const GLvoid *)offsetof(glimac::ShapeVertex, position));
  glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,
                        sizeof(glimac::ShapeVertex),
                        (const GLvoid *)offsetof(glimac::ShapeVertex, normal));
  glVertexAttribPointer(
      VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex),
      (const GLvoid *)offsetof(glimac::ShapeVertex, texCoords));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}