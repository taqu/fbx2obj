#include "Shape.h"
#include <cassert>
#include <unordered_set>
#include <fbxsdk.h>
#include "Mesh.h"

namespace std
{
template<>
class hash<fbx2obj::Shape::Vertex>
{
public:
    size_t operator()(const fbx2obj::Shape::Vertex& x) const
    {
        static constexpr uint64_t Prime = 0x00000100000001b3ULL;
        static constexpr uint64_t Offset = 0xcbf29ce484222325ULL;
        uint64_t hash = Offset;
        const uint8_t* p = reinterpret_cast<const uint8_t*>(&x);
        for(uint64_t i = 0; i < sizeof(fbx2obj::Shape::Vertex); ++i) {
            hash = hash ^ p[i];
            hash = hash * Prime;
        }
        return hash;
    }
};
} // namespace std

namespace fbx2obj
{
namespace
{
    Vector2 make2(const fbxsdk::FbxVector2& x)
    {
        return Vector2(static_cast<float>(x[0]), static_cast<float>(x[1]));
    }

    Vector3 make3(const fbxsdk::FbxVector4& x)
    {
        return Vector3(static_cast<float>(x[0]), static_cast<float>(x[1]), static_cast<float>(x[2]));
    }
}

bool operator==(const Shape::Vertex& x0, const Shape::Vertex& x1)
{
    return equals(x0.position_, x1.position_) && equals(x0.normal_, x1.normal_);
}

Shape::Shape()
    :meshNumVertices_(0)
{
}

Shape::~Shape()
{
}

const std::string& Shape::getName() const
{
    return name_;
}

    int32_t Shape::getMeshNumVertices() const
{
        return meshNumVertices_;
}

    const std::vector<Shape::Vertex>& Shape::getVertices() const
{
        return vertices_;
}

    const std::unordered_map<int32_t, int32_t>& Shape::getMeshToShape() const
{
        return meshToShape_;
}

void Shape::parse(const fbxsdk::FbxShape* fbxShape, const fbxsdk::FbxMesh* fbxMesh, const Mesh& mesh)
{
    assert(nullptr != fbxShape);
    name_ = fbxShape->GetName();
    meshNumVertices_ = mesh.vertices_.size();
    vertices_.clear();
    meshToShape_.clear();

    int32_t meshControlPointCount = fbxMesh->GetControlPointsCount();
    int32_t shapeControlPointCount = fbxShape->GetControlPointsCount();
    std::unordered_set<int32_t> controlPointSet;
    for(int32_t i=0; i<fbxShape->GetControlPointIndicesCount(); ++i){
        controlPointSet.insert(fbxShape->GetControlPointIndices()[i]);
    }
    std::unordered_map<Vertex, int32_t> vertices;
    const FbxGeometryElementNormal* normalElement = fbxShape->GetElementNormal(0);

    for(int32_t i=0; i<mesh.vertexToControlPoint_.size(); ++i){
        int32_t meshControlPoint = mesh.vertexToControlPoint_[i];
        if(!controlPointSet.contains(meshControlPoint)) {
            continue;
        }
        FbxVector4 normal(0.0, 0.0, 0.0, 0.0);
        if(normalElement) {
            if(normalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
                int32_t normalIndex = (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
                                          ? meshControlPoint
                                          : normalElement->GetIndexArray().GetAt(meshControlPoint);
                normal = normalElement->GetDirectArray().GetAt(normalIndex);
            }
        }

        Vertex vertex;
        vertex.position_ = make3(fbxShape->GetControlPointAt(meshControlPoint));
        vertex.normal_ = make3(normal);

        int32_t dstIndex = 0;
        if(vertices.contains(vertex)) {
            dstIndex = vertices.at(vertex);
        } else {
            dstIndex = vertices_.size();
            vertices_.push_back(vertex);
            vertices.insert(std::make_pair(vertex, dstIndex));
        }
        int32_t meshDstIndex = i;
        meshToShape_.insert(std::make_pair(meshDstIndex, dstIndex));
    }
}
} // namespace fbx2obj
