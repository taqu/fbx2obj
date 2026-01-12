#ifndef INC_FBX2OBJ_NODE_H
#define INC_FBX2OBJ_NODE_H
#include <vector>
#include "fbx2obj.h"
#include "Mesh.h"

namespace fbxsdk
{
    class FbxNode;
}

namespace fbx2obj
{
    enum class NodeAttrType
    {   
        Unknown,
        Null,
        Marker,
        Skeleton, 
        Mesh, 
        Nurbs, 
        Patch,
        Camera, 
        CameraStereo,
        CameraSwitcher,
        Light,
        OpticalReference,
        OpticalMarker,
        NurbsCurve,
        TrimNurbsSurface,
        Boundary,
        NurbsSurface,
        Shape,
        LODGroup,
        SubDiv,
        CachedEffect,
        Line,
    };

    class Node
{
public:
    Node();
    ~Node();
    Node(const Node&) = default;
    Node& operator=(const Node&) = default;

    const std::vector<Mesh>& getMeshes() const;

    void parse(int32_t index, int32_t parent, int32_t childStart, const fbxsdk::FbxNode* fbxNode);
private:
    void parseMesh(const fbxsdk::FbxNode* fbxNode);
    int32_t index_;
    int32_t parent_;
    int32_t childStart_;
    int32_t numChildren_;
    NodeAttrType nodeAttrType_;
    std::vector<Mesh> meshes_;
};

}
#endif //INC_FBX2OBJ_NODE_H

