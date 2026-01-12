#include "Node.h"
#include <cassert>
#include <fbxsdk.h>

namespace fbx2obj
{
Node::Node()
{
}

Node::~Node()
{
}

const std::vector<Mesh>& Node::getMeshes() const
{
    return meshes_;
}

void Node::parse(int32_t index, int32_t parent, int32_t childStart, const fbxsdk::FbxNode* fbxNode)
{
    assert(nullptr != fbxNode);
    index_ = index;
    parent_ = parent;
    childStart_ = childStart;
    numChildren_ = fbxNode->GetChildCount();
    if(nullptr == fbxNode->GetNodeAttribute()) {
        nodeAttrType_ = NodeAttrType::Unknown;
        return;
    }
    switch(fbxNode->GetNodeAttribute()->GetAttributeType()) {
    case fbxsdk::FbxNodeAttribute::eNull:
        nodeAttrType_ = NodeAttrType::Null;
        break;
    case fbxsdk::FbxNodeAttribute::eMarker:
        nodeAttrType_ = NodeAttrType::Marker;
        break;
    case fbxsdk::FbxNodeAttribute::eSkeleton:
        nodeAttrType_ = NodeAttrType::Skeleton;
        break;
    case fbxsdk::FbxNodeAttribute::eMesh:
        nodeAttrType_ = NodeAttrType::Mesh;
        parseMesh(fbxNode);
        break;
    case fbxsdk::FbxNodeAttribute::eNurbs:
        nodeAttrType_ = NodeAttrType::Nurbs;
        break;
    case fbxsdk::FbxNodeAttribute::ePatch:
        nodeAttrType_ = NodeAttrType::Patch;
        break;
    case fbxsdk::FbxNodeAttribute::eCamera:
        nodeAttrType_ = NodeAttrType::Camera;
        break;
    case fbxsdk::FbxNodeAttribute::eCameraStereo:
        nodeAttrType_ = NodeAttrType::CameraStereo;
        break;
    case fbxsdk::FbxNodeAttribute::eCameraSwitcher:
        nodeAttrType_ = NodeAttrType::CameraSwitcher;
        break;
    case fbxsdk::FbxNodeAttribute::eLight:
        nodeAttrType_ = NodeAttrType::Light;
        break;
    case fbxsdk::FbxNodeAttribute::eOpticalReference:
        nodeAttrType_ = NodeAttrType::OpticalReference;
        break;
    case fbxsdk::FbxNodeAttribute::eOpticalMarker:
        nodeAttrType_ = NodeAttrType::OpticalMarker;
        break;
    case fbxsdk::FbxNodeAttribute::eNurbsCurve:
        nodeAttrType_ = NodeAttrType::NurbsCurve;
        break;
    case fbxsdk::FbxNodeAttribute::eTrimNurbsSurface:
        nodeAttrType_ = NodeAttrType::TrimNurbsSurface;
        break;
    case fbxsdk::FbxNodeAttribute::eBoundary:
        nodeAttrType_ = NodeAttrType::Boundary;
        break;
    case fbxsdk::FbxNodeAttribute::eNurbsSurface:
        nodeAttrType_ = NodeAttrType::NurbsSurface;
        break;
    case fbxsdk::FbxNodeAttribute::eShape:
        nodeAttrType_ = NodeAttrType::Shape;
        break;
    case fbxsdk::FbxNodeAttribute::eLODGroup:
        nodeAttrType_ = NodeAttrType::LODGroup;
        break;
    case fbxsdk::FbxNodeAttribute::eSubDiv:
        nodeAttrType_ = NodeAttrType::SubDiv;
        break;
    case fbxsdk::FbxNodeAttribute::eCachedEffect:
        nodeAttrType_ = NodeAttrType::CachedEffect;
        break;
    case fbxsdk::FbxNodeAttribute::eLine:
        nodeAttrType_ = NodeAttrType::Line;
        break;
    default:
        nodeAttrType_ = NodeAttrType::Unknown;
        break;
    }
}

void Node::parseMesh(const fbxsdk::FbxNode* fbxNode)
{
    const fbxsdk::FbxNodeAttribute* fbxAttr = fbxNode->GetNodeAttribute();
    Mesh mesh;
    mesh.parse((const fbxsdk::FbxMesh*)fbxAttr);
    meshes_.push_back(mesh);
}
} // namespace fbx2obj
