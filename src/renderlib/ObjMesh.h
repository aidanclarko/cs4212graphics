#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "vec3.h"
#include "Triangle.h"

class ObjMesh {
    public:
        ObjMesh() {}
        ObjMesh(std::string fileName) : fileName(fileName) {

            std::ifstream file(fileName);
            if (!file) {
                std::cerr << "Failed to open file\n";
                return;
            }

            std::string line;
            while (std::getline(file, line)) {
                std::vector<std::string> res = split(line, ' ');
                if (res.empty()) continue;
                
                if (res[0] == "v" && res.size() >= 4) {
                    verticies.push_back(vec3(
                        std::stof(res[1]),
                        std::stof(res[2]),
                        std::stof(res[3])
                    ));
                }
                if (res[0] == "f" && res.size() >= 4) {
                    faces.push_back(Triangle(
                        verticies.at(std::stoi(res[1]) - 1),
                        verticies.at(std::stoi(res[2]) - 1),
                        verticies.at(std::stoi(res[3]) - 1)
                    ));
                }
            }
        }

        std::vector<Triangle>& getFaces()  { return faces; }
    private:
        std::string fileName;
        std::vector<vec3> verticies;
        std::vector<Triangle> faces;

        std::vector<std::string> split(const std::string& s, char delim) {
            std::vector<std::string> result;
            std::stringstream ss(s);
            std::string item;

            while (std::getline(ss, item, delim)) {
                result.push_back(item);
            }
            return result;
        }

};