#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "glimac/common.hpp"
#include "glm/fwd.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../tiny_obj_loader.h"

class Model {
public:
    Model(const std::string& filename)
    {
        std::string err;
        std::string warn;

        std::string objFile = "assets/models/" + filename;
        // std::string mtlFile = "/home/pauline/Documents/S4/Papeterie/assets/models/";

        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objFile.c_str());

        if (!warn.empty())
        {
            std::cerr << warn << std::endl;
        }

        if (!err.empty())
        {
            throw std::runtime_error(err);
        }

        if (!ret)
        {
            throw std::runtime_error("Failed to load OBJ file");
        }

        for (const auto& shape : shapes)
        {
            size_t index_offset = 0;
            for (size_t i = 0; i < shape.mesh.num_face_vertices.size(); i++)
            {
                size_t fv = shape.mesh.num_face_vertices[i];

                for (size_t v = 0; v < fv; v++)
                {
                    tinyobj::index_t index = shape.mesh.indices[index_offset + v];

                    glimac::ShapeVertex vertex;

                    vertex.position = glm::vec3(
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2]
                    );

                    vertex.normal = glm::vec3(
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]
                    );

                    vertex.texCoords = glm::vec2(
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        attrib.texcoords[2 * index.texcoord_index + 1]
                    );

                    vertices.push_back(vertex);
                }
                index_offset += fv;
            }
        }
    }

    const std::vector<glimac::ShapeVertex>& getVertices() const
    {
        return vertices;
    }

    const tinyobj::attrib_t& getAttrib() const
    {
        return attrib;
    }

    const std::vector<tinyobj::shape_t>& getShapes() const
    {
        return shapes;
    }

    const std::vector<tinyobj::material_t>& getMaterials() const
    {
        return materials;
    }

private:
    tinyobj::attrib_t                attrib;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;
    std::vector<glimac::ShapeVertex> vertices;
};
