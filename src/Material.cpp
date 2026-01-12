#include "Material.h"
#include <cassert>
#include <fbxsdk.h>

namespace fbx2obj
{
namespace
{
    Vector4 get(const fbxsdk::FbxDouble3& color, fbxsdk::FbxDouble factor)
    {
        Vector4 r;
        r.x_ = static_cast<float>(color[0]);
        r.y_ = static_cast<float>(color[1]);
        r.z_ = static_cast<float>(color[2]);
        r.w_ = static_cast<float>(factor);
        return r;
    }
} // namespace

Material::Material()
{
}

Material::~Material()
{
}

const std::string& Material::getName() const
{
    return name_;
}

const Vector4& Material::getAmbient() const
{
    return ambient_;
}

const Vector4& Material::getDiffuse() const
{
    return diffuse_;
}

const Vector4& Material::getEmission() const
{
    return emission_;
}

const Vector4& Material::getTransparent() const
{
    return transparent_;
}

const Vector4& Material::getReflection() const
{
    return reflection_;
}

const Vector4& Material::getSpecular() const
{
    return specular_;
}

float Material::getShininess() const
{
    return shininess_;
}

const std::string& Material::getTextureDiffuse() const
{
    return textureDiffuse_;
}

const std::string& Material::getTextureNormal() const
{
    return textureNormal_;
}

void Material::parse(const fbxsdk::FbxSurfaceMaterial* fbxMaterial)
{
    assert(nullptr != fbxMaterial);
    name_ = fbxMaterial->GetName();
    if(fbxMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId)) {
        const fbxsdk::FbxSurfaceLambert* lambert = static_cast<const fbxsdk::FbxSurfaceLambert*>(fbxMaterial);
        ambient_ = get(lambert->Ambient, lambert->AmbientFactor.Get());
        diffuse_ = get(lambert->Diffuse, lambert->DiffuseFactor.Get());
        emission_ = get(lambert->Emissive, lambert->EmissiveFactor.Get());
        transparent_ = get(lambert->TransparentColor, lambert->TransparencyFactor.Get());
    } else if(fbxMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId)) {
        const fbxsdk::FbxSurfacePhong* phong = static_cast<const fbxsdk::FbxSurfacePhong*>(fbxMaterial);
        ambient_ = get(phong->Ambient, phong->AmbientFactor.Get());
        diffuse_ = get(phong->Diffuse, phong->DiffuseFactor.Get());
        emission_ = get(phong->Emissive, phong->EmissiveFactor.Get());
        transparent_ = get(phong->TransparentColor, phong->TransparencyFactor.Get());
        reflection_ = get(phong->Reflection, phong->ReflectionFactor.Get());
        specular_ = get(phong->Specular, phong->SpecularFactor.Get());
        shininess_ = static_cast<float>(phong->Shininess.Get());
    }

    {
        fbxsdk::FbxProperty property = fbxMaterial->FindProperty(fbxsdk::FbxSurfaceMaterial::sDiffuse);
        int32_t numLayers = property.GetSrcObjectCount<fbxsdk::FbxLayeredTexture>();
        if(numLayers <= 0) {
            int32_t numTextures = property.GetSrcObjectCount<fbxsdk::FbxFileTexture>();
            if(0 < numTextures) {
                const fbxsdk::FbxFileTexture* texture = property.GetSrcObject<fbxsdk::FbxFileTexture>(0);
                textureDiffuse_ = texture->GetRelativeFileName();
            }
        }

        property = fbxMaterial->FindProperty(fbxsdk::FbxSurfaceMaterial::sNormalMap);
        numLayers = property.GetSrcObjectCount<fbxsdk::FbxLayeredTexture>();
        if(numLayers <= 0) {
            int32_t numTextures = property.GetSrcObjectCount<fbxsdk::FbxFileTexture>();
            if(0 < numTextures) {
                const fbxsdk::FbxFileTexture* texture = property.GetSrcObject<fbxsdk::FbxFileTexture>(0);
                textureNormal_ = texture->GetRelativeFileName();
            }
        }
    }
}
} // namespace fbx2obj
