#include "Scene.h"
#include <cassert>
#include <fbxsdk.h>
#include <filesystem>

namespace fbx2obj
{
Scene::Scene()
{
}

Scene::~Scene()
{
}

const std::string& Scene::getName() const
{
    return name_;
}

const std::vector<Node>& Scene::getNodes() const
{
    return nodes_;
}

int32_t Scene::countMaterials() const
{
    int32_t count = 0;
    for(const Node& node : nodes_) {
        for(const Mesh& mesh : node.getMeshes()) {
            if(!mesh.getMaterial().getName().empty()){
                ++count;
            }
        }
    }
    return count;
}

    Scene Scene::load(const char* filepath)
{
    assert(nullptr != filepath);
    std::filesystem::path path(filepath);
    fbxsdk::FbxManager* fbxManager = FbxManager::Create();
    fbxsdk::FbxIOSettings* fbxIOSettings = fbxsdk::FbxIOSettings::Create(fbxManager, IOSROOT);
    fbxManager->SetIOSettings(fbxIOSettings);
    fbxsdk::FbxScene* fbxScene = fbxsdk::FbxScene::Create(fbxManager, path.stem().string().c_str());
    fbxsdk::FbxImporter* importer = fbxsdk::FbxImporter::Create(fbxManager, "");
    importer->Initialize(filepath, -1, fbxManager->GetIOSettings());
    bool result = importer->Import(fbxScene);
    importer->Destroy();
    Scene scene;
    if(!result) {
        fbxScene->Destroy();
        return scene;
    }
    fbxsdk::FbxGeometryConverter geometryConverter(fbxManager);
    geometryConverter.Triangulate(fbxScene, true);
    geometryConverter.RemoveBadPolygonsFromMeshes(fbxScene);
    geometryConverter.SplitMeshesPerMaterial(fbxScene, true);

    scene.name_ = fbxScene->GetName();
    scene.nodes_.reserve(fbxScene->GetNodeCount());
    if(nullptr !=fbxScene->GetRootNode()){
        scene.nodes_.resize(1);
        scene.traverse(-1, 0, fbxScene->GetRootNode());
    }
    fbxScene->Destroy();
    fbxManager->Destroy();
    return scene;
}

void Scene::traverse(int32_t parent, int32_t index, const fbxsdk::FbxNode* fbxNode)
{
    int32_t childStart = static_cast<int32_t>(nodes_.size());
    nodes_.resize(nodes_.size() + fbxNode->GetChildCount());
    nodes_[index].parse(index, parent, childStart, fbxNode);
    for(int32_t i=0; i<fbxNode->GetChildCount(); ++i){
        traverse(index, childStart+i, fbxNode->GetChild(i));
    }
}

} // namespace fbx2obj
