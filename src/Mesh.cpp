#include "Mesh.h"
#include <cassert>
#ifdef _DEBUG
#    include <cstdio>
#endif
#include <fbxsdk.h>
#include <unordered_map>
#include <utility>

namespace std
{
template<>
class hash<fbx2obj::Vertex>
{
public:
    size_t operator()(const fbx2obj::Vertex& x) const
    {
        static constexpr uint64_t Prime = 0x00000100000001b3ULL;
        static constexpr uint64_t Offset = 0xcbf29ce484222325ULL;
        uint64_t hash = Offset;
        const uint8_t* p = reinterpret_cast<const uint8_t*>(&x);
        for(uint64_t i = 0; i < sizeof(fbx2obj::Vertex); ++i) {
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
} // namespace

bool operator==(const Vertex& x0, const Vertex& x1)
{
    return equals(x0.position_, x1.position_) && equals(x0.normal_, x1.normal_) && equals(x0.texcoord0_, x1.texcoord0_);
}

bool operator!=(const Vertex& x0, const Vertex& x1)
{
    return !equals(x0.position_, x1.position_) || !equals(x0.normal_, x1.normal_) || !equals(x0.texcoord0_, x1.texcoord0_);
}

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}
const std::string& Mesh::getName() const
{
    return name_;
}

const std::vector<Vertex>& Mesh::getVertices() const
{
    return vertices_;
}

const std::vector<int32_t>& Mesh::getIndices() const
{
    return indices_;
}

const Material& Mesh::getMaterial() const
{
    return material_;
}

const std::vector<Shape>& Mesh::getShapes() const
{
    return shapes_;
}

void Mesh::parse(const fbxsdk::FbxMesh* fbxMesh)
{
    vertices_.clear();
    indices_.clear();
    vertexToControlPoint_.clear();
    shapes_.clear();

    name_ = fbxMesh->GetName();
    FbxStringList uvSetNames;
    fbxMesh->GetUVSetNames(uvSetNames);

    std::vector<Vector3> positions;
    positions.reserve(fbxMesh->GetControlPointsCount());
    for(int32_t i = 0; i < fbxMesh->GetControlPointsCount(); ++i) {
        const fbxsdk::FbxVector4 point = fbxMesh->GetControlPointAt(i);
        positions.push_back(make3(point));
    }

    int32_t index = 0;
    std::unordered_map<Vertex, int32_t> vertices;
    for(int32_t polygon = 0; polygon < fbxMesh->GetPolygonCount(); ++polygon) {
        assert(3 == fbxMesh->GetPolygonSize(polygon));
        for(int32_t indexInPolygon = 0; indexInPolygon < 3; ++indexInPolygon) {
            int32_t controlPoint = fbxMesh->GetPolygonVertex(polygon, indexInPolygon);
            if(controlPoint < 0 || fbxMesh->GetControlPointsCount() <= controlPoint) {
                continue;
            }
            fbxsdk::FbxVector4 normal(0.0, 0.0, 0.0);
            fbxMesh->GetPolygonVertexNormal(polygon, indexInPolygon, normal);
            fbxsdk::FbxVector2 uv(0.0, 0.0);
            if(0 < uvSetNames.GetCount()) {
                bool mapped;
                fbxMesh->GetPolygonVertexUV(polygon, indexInPolygon, uvSetNames[0], uv, mapped);
            }
            Vertex vertex;
            vertex.position_ = positions[controlPoint];
            vertex.normal_ = make3(normal);
            vertex.texcoord0_ = make2(uv);

            int32_t dstIndex = 0;
            if(vertices.contains(vertex)) {
                dstIndex = vertices.at(vertex);
            } else {
                dstIndex = static_cast<int32_t>(vertices_.size());
                vertices_.push_back(vertex);
                vertices.insert(std::make_pair(vertex, dstIndex));
                vertexToControlPoint_.push_back(controlPoint);
            }
            indices_.push_back(dstIndex);
            ++index;
        }
    }

    for(int32_t i = 0; i < fbxMesh->GetDeformerCount(FbxDeformer::eBlendShape); ++i) {
        FbxBlendShape* blendShape = static_cast<FbxBlendShape*>(fbxMesh->GetDeformer(i, FbxDeformer::eBlendShape));
        int32_t channelCount = blendShape->GetBlendShapeChannelCount();
        for(int32_t j = 0; j < channelCount; ++j) {
            FbxBlendShapeChannel* blendShapeChannel = blendShape->GetBlendShapeChannel(j);
            if(nullptr == blendShapeChannel) {
                continue;
            }
            //blendShapeChannel->DeformPercent.Set(100);
#ifdef _DEBUG
            const char* channelName = blendShapeChannel->GetName();
            double currentWeight = blendShapeChannel->DeformPercent.Get();
            printf("Channel [%d]: %s | Weight: %f\n", j, channelName, currentWeight);
#endif

            int32_t targetShapeCount = blendShapeChannel->GetTargetShapeCount();
            for(int32_t k = 0; k < targetShapeCount; ++k) {
                FbxShape* fbxShape = blendShapeChannel->GetTargetShape(k);
                Shape shape;
                shape.parse(fbxShape, fbxMesh, *this);
                shapes_.push_back(std::move(shape));
            }
        }
    }

    const fbxsdk::FbxNode* node = fbxMesh->GetNode();
    if(nullptr == node) {
        return;
    }
    if(node->GetMaterialCount() <= 0) {
        return;
    }
    assert(node->GetMaterialCount() == 1);
    const fbxsdk::FbxSurfaceMaterial* material = node->GetMaterial(0);
    if(nullptr == material) {
        return;
    }
    material_.parse(material);
}
} // namespace fbx2obj
