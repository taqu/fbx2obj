#ifndef INC_FBX2OBJ_MESH_H
#define INC_FBX2OBJ_MESH_H
#include <string>
#include <vector>
#include "fbx2obj.h"
#include "Material.h"
#include "Shape.h"

namespace fbxsdk
{
    class FbxMesh;
}

namespace fbx2obj
{
    struct Vertex
    {
        Vector3 position_;
        Vector3 normal_;
        Vector2 texcoord0_;
    };
    bool operator==(const Vertex& x0, const Vertex& x1);
    bool operator!=(const Vertex& x0, const Vertex& x1);

     class Mesh
{
public:
    Mesh();
    ~Mesh();
    Mesh(const Mesh&) = default;
    Mesh& operator=(const Mesh&) = default;
    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;

    const std::string& getName() const;
    const std::vector<Vertex>& getVertices() const;
    const std::vector<int32_t>& getIndices() const;
    const Material& getMaterial() const;
    const std::vector<Shape>& getShapes() const;

    void parse(const fbxsdk::FbxMesh* fbxMesh);
private:
    friend class Shape;
    std::string name_;
    std::vector<Vertex> vertices_;
    std::vector<int32_t> indices_;
    std::vector<int32_t> vertexToControlPoint_;
    Material material_;
    std::vector<Shape> shapes_;
};
}
#endif //INC_FBX2OBJ_MESH_H

