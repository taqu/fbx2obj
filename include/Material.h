#ifndef INC_FBX2OBJ_MATERIAL_H
#define INC_FBX2OBJ_MATERIAL_H
#include "fbx2obj.h"
#include <string>

namespace fbxsdk
{
    class FbxSurfaceMaterial;
}

namespace fbx2obj
{
class Material
{
public:
    Material();
    ~Material();
    Material(const Material&) = default;
    Material& operator=(const Material&) = default;
    Material(Material&&) = default;
    Material& operator=(Material&&) = default;

    const std::string& getName() const;
    const Vector4& getAmbient() const;
    const Vector4& getDiffuse() const;
    const Vector4& getEmission() const;
    const Vector4& getTransparent() const;
    const Vector4& getReflection() const;
    const Vector4& getSpecular() const;
    float getShininess() const;
    const std::string& getTextureDiffuse() const;
    const std::string& getTextureNormal() const;

    void parse(const fbxsdk::FbxSurfaceMaterial* fbxMaterial);

    std::string name_;
    Vector4 ambient_ = {};
    Vector4 diffuse_ = {};
    Vector4 emission_ = {};
    Vector4 transparent_ = {};
    Vector4 reflection_ = {};
    Vector4 specular_ = {};
    float shininess_ = 0.0f;
    std::string textureDiffuse_;
    std::string textureNormal_;
};
} // namespace fbx2obj
#endif // INC_FBX2OBJ_MATERIAL_H
