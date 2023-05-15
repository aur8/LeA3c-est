#include "Model.hpp"
#include <string.h>
#include <cstdio>
#include <fstream>
#include <iostream>

Model::Model(){

};

void Model::load_from_file(const char* filename)
{
}

std::vector<float> Model::get_vertex_data()
{
}

int Model::get_vertex_count()
{
}

void Model::load_material_file(const char* filename)
{
    std::ifstream file(filename);
    if (file)
    {
        std::string line;
        while (std::getline(file, line))
        {
            char mtl_name(100);
            if (start_with(line, "normal"))
            {
                (void)sscanf(const char* __restrict s, const char* __restrict format)
            }
            if (start_with(line, "Kd"))
            {
            }
        }
    }
    else
    {
        std::cout << "Matérial file failed \n";
    }
}

bool Model::start_with(std::string& line, const char* text)
{
    size_t text_len = strlen(text);
    if (line.size() < text_len)
    {
        return false;
    }
    for (size_t i = 0; i < text_len; i++)
    {
        if (line[i] == text[i])
            continue;
        else
            return false;
    }
    return true;
}

void Model::add_vertex_data(int v1, int v2, int v3, const char* mtl, int n1, int n2, int n3)
{
}
