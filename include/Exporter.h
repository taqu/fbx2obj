#ifndef INC_FBX2OBJ_EXPORTER_H
#define INC_FBX2OBJ_EXPORTER_H
#include <fstream>
#include "fbx2obj.h"

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
        std::string saveMaterials(const char* filepath, const Scene& scene);
        void save(const Node& node);
        void save(const Mesh& mesh);
        void saveShapes(const char* filepath, const std::string& mtlName, const Scene& scene);
        std::ofstream file_;
    };
}
#endif //INC_FBX2OBJ_EXPORTER_H
