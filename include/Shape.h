#ifndef INC_FBX2OBJ_SHAPE_H
#define INC_FBX2OBJ_SHAPE_H
#include <unordered_map>
#include <string>
#include <vector>
#include "fbx2obj.h"

namespace fbxsdk
{
    class FbxMesh;
    class FbxShape;
}

namespace fbx2obj
{
    class Mesh;

class Shape
{
public:
    struct Vertex
    {
        Vector3 position_;
        Vector3 normal_;
    };

    Shape();
    ~Shape();
    Shape(const Shape&) = default;
    Shape& operator=(const Shape&) = default;
    Shape(Shape&&) = default;
    Shape& operator=(Shape&&) = default;

    const std::string& getName() const;
    int32_t getMeshNumVertices() const;
    const std::vector<Vertex>& getVertices() const;
    const std::unordered_map<int32_t, int32_t>& getMeshToShape() const;

    void parse(const fbxsdk::FbxShape* fbxShape, const fbxsdk::FbxMesh* fbxMesh, const Mesh& mesh);
private:
    std::string name_;
    int32_t meshNumVertices_;
    std::vector<Vertex> vertices_;
    std::unordered_map<int32_t, int32_t> meshToShape_;
};

bool operator==(const Shape::Vertex& x0, const Shape::Vertex& x1);
} // namespace fbx2obj
#endif //INC_FBX2OBJ_SHAPE_H