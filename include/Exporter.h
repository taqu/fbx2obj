#ifndef INC_FBX2OBJ_EXPORTER_H
#define INC_FBX2OBJ_EXPORTER_H
#include <vector>
#include <tuple>
#include <fstream>
#include "fbx2obj.h"
#include "Mesh.h"

namespace fbx2obj
{
    class Scene;
    class Node;
    class Mesh;
    class Shape;

    class Exporter
{
    public:
        Exporter();
        ~Exporter();

        void save(const char* filepath, const Scene& scene);
    private:
        Exporter(const Exporter&) = delete;
        Exporter& operator=(const Exporter&) = delete;
        void collectShapes(const Scene& scene);
        std::string saveMaterials(const char* filepath, const Scene& scene);
        void saveScene(const char* filepath, const Scene& scene);
        void saveNode(const Node& node);
        void saveMesh(const Mesh& mesh, const std::vector<Vertex>& vertices);
        void saveMesh(const Mesh& mesh);
        void saveShapes(const char* filepath, const Scene& scene);
        std::string mtlName_;
        std::ofstream file_;
        size_t globalIndex_;
        std::vector<std::tuple<const Shape*, const Mesh*>> shapes_;
    };
}
#endif //INC_FBX2OBJ_EXPORTER_H
