#ifndef INC_FBX2OBJ_SCENE_H
#define INC_FBX2OBJ_SCENE_H
#include <vector>
#include "fbx2obj.h"
#include "Node.h"

namespace fbxsdk
{
    class FbxNode;
}

namespace fbx2obj
{
class Scene
{
public:
    Scene();
    ~Scene();

    const std::string& getName() const;
    const std::vector<Node>& getNodes() const;
    int32_t countMaterials() const;

    static Scene load(const char* filepath);
private:
    Scene(const Scene&) = default;
    Scene& operator=(const Scene&) = default;

    void traverse(int32_t parent, int32_t index, const fbxsdk::FbxNode* fbxNode);
    std::string name_;
    std::vector<Node> nodes_;
};
} // namespace fbx2obj
#endif //INC_FBX2OBJ_SCENE_H

