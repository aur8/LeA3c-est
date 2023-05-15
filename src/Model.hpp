#include <map>
#include <string>
#include <vector>
#include "glm/fwd.hpp"

class Model {
public:
    Model();

    void               load_from_file(const char* filename);
    std::vector<float> get_vertex_data();
    int                get_vertex_count();

private:
    glm::vec3 m_position;
    glm::vec3 m_color;
    glm::vec3 m_normals;

    void load_material_file(const char* filename);
    bool start_with(std::string& line, const char* text);
    void add_vertex_data(int v1, int v2, int v3, const char* mtl, int n1, int n2, int n3);

    std::map<std::string, glm::vec3> m_material_map;
    std::vector<float>               m_vertex_map;
};