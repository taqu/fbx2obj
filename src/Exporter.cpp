#include "Exporter.h"
#include "Scene.h"
#include <algorithm>
#include <cassert>
#include <filesystem>

namespace fbx2obj
{
namespace
{
    Vector3 applyFactor(const Vector4& x)
    {
        Vector3 r;
        #if 0
        r.x_ = x.x_ * x.w_;
        r.y_ = x.y_ * x.w_;
        r.z_ = x.z_ * x.w_;
        #else
        r.x_ = x.x_;
        r.y_ = x.y_;
        r.z_ = x.z_;
        #endif
        return r;
    }
} // namespace

Exporter::Exporter()
{
}

Exporter::~Exporter()
{
}

void Exporter::save(const char* filepath, const Scene& scene)
{
    assert(nullptr != filepath);
    if(0 < scene.countMaterials()) {
        mtlName_ = saveMaterials(filepath, scene);
    }
    collectShapes(scene);
    saveScene(filepath, scene);
    saveShapes(filepath, scene);
}

void Exporter::collectShapes(const Scene& scene)
{
    shapes_.clear();
    for(const Node& node: scene.getNodes()) {
        for(const Mesh& mesh: node.getMeshes()) {
            for(const Shape& shape: mesh.getShapes()) {
                shapes_.push_back(std::make_tuple(&shape, &mesh));
            }
        }
    }
}

std::string Exporter::saveMaterials(const char* filepath, const Scene& scene)
{
    assert(nullptr != filepath);
    std::filesystem::path path(filepath);
    path = path.replace_extension("mtl");
    std::ofstream file(path.string().c_str(), std::ios::binary);
    if(!file.is_open()) {
        return "";
    }
    for(const Node& node: scene.getNodes()) {
        for(const Mesh& mesh: node.getMeshes()) {
            const Material& material = mesh.getMaterial();
            if(material.getName().empty()) {
                continue;
            }
            file << "newmtl " << material.getName() << "\n";
            const Vector3 ambient = applyFactor(material.getAmbient());
            file << "Ka " << ambient.x_ << " " << ambient.y_ << " " << ambient.z_ << "\n";

            const Vector3 diffuse = applyFactor(material.getDiffuse());
            file << "Kd " << diffuse.x_ << " " << diffuse.y_ << " " << diffuse.z_ << "\n";

            const Vector3 emission = applyFactor(material.getEmission());
            file << "Ke " << emission.x_ << " " << emission.y_ << " " << emission.z_ << "\n";

            const Vector3 specular = applyFactor(material.getSpecular());
            file << "Ks " << specular.x_ << " " << specular.y_ << " " << specular.z_ << "\n";

            const Vector4& transparent = material.getTransparent();
            file << "Tf " << transparent.x_ << " " << transparent.y_ << " " << transparent.z_ << "\n";
            file << "d " << std::clamp(1.0f - transparent.w_, 0.0f, 1.0f) << "\n";

            file << "Ns " << material.getShininess() << "\n";

            if(!material.getTextureDiffuse().empty()) {
                file << "map_Kd " << material.getTextureDiffuse() << "\n";
            }
            if(!material.getTextureEmission().empty()) {
                file << "map_Ke " << material.getTextureEmission() << "\n";
            }
            if(!material.getTextureSpecular().empty()) {
                file << "map_Ks " << material.getTextureSpecular() << "\n";
            }
            if(!material.getTextureNormal().empty()) {
                file << "map_Bump " << material.getTextureNormal() << "\n";
            }
        }
    }
    file.close();
    return path.filename().string();
}

void Exporter::saveScene(const char* filepath, const Scene& scene)
{
    file_.open(filepath, std::ios::binary);
    if(!file_.is_open()) {
        return;
    }
    if(!mtlName_.empty()) {
        file_ << "mtllib " << mtlName_ << "\n";
    }
    globalIndex_ = 1;
    for(size_t i = 0; i < scene.getNodes().size(); ++i) {
        saveNode(scene.getNodes()[i]);
    }
    file_.close();
}

void Exporter::saveNode(const Node& node)
{
    const std::vector<Mesh>& meshes = node.getMeshes();
    for(size_t i = 0; i < meshes.size(); ++i) {
        saveMesh(meshes[i]);
    }
}

void Exporter::saveMesh(const Mesh& mesh, const std::vector<Vertex>& vertices)
{
    if(!mesh.getName().empty()) {
        file_ << "g " << mesh.getName() << "\n";
    }
    if(!mesh.getMaterial().getName().empty()) {
        file_ << "usemtl " << mesh.getMaterial().getName() << "\n";
    }
    for(const Vertex& v: vertices) {
        file_ << "v " << v.position_.x_ << " " << v.position_.y_ << " " << v.position_.z_ << "\n";
    }
    for(const Vertex& v: vertices) {
        file_ << "vt " << v.texcoord0_.x_ << " " << v.texcoord0_.y_ << "\n";
    }
    for(const Vertex& v: vertices) {
        file_ << "vn " << v.normal_.x_ << " " << v.normal_.y_ << " " << v.normal_.z_ << "\n";
    }

    const std::vector<int32_t>& indices = mesh.getIndices();
    for(size_t i = 0; i < indices.size(); i += 3) {
        file_ << "f ";
        size_t i0 = indices[i + 0] + globalIndex_;
        size_t i1 = indices[i + 1] + globalIndex_;
        size_t i2 = indices[i + 2] + globalIndex_;
        file_ << i0 << "/" << i0 << "/" << i0 << " ";
        file_ << i1 << "/" << i1 << "/" << i1 << " ";
        file_ << i2 << "/" << i2 << "/" << i2 << "\n";
    }
    globalIndex_ += vertices.size();
}

void Exporter::saveMesh(const Mesh& mesh)
{
    saveMesh(mesh, mesh.getVertices());
}

void Exporter::saveShapes(const char* filepath, const Scene& scene)
{
    std::filesystem::path basePath(filepath);
    std::string baseName = basePath.stem().string();
    basePath = basePath.parent_path();
    for(const std::tuple<const Shape*, const Mesh*>& pair: shapes_) {
        std::string fileName = baseName + "_" + std::get<0>(pair)->getName() + ".obj";
        std::filesystem::path path = basePath.string() + "/" + fileName;
        path.make_preferred();
        file_.open(path, std::ios::binary);
        if(!file_.is_open()) {
            continue;
        }
        globalIndex_ = 1;
        file_ << "mtllib " << mtlName_ << "\n";

        for(const Node& node: scene.getNodes()) {
            for(const Mesh& mesh: node.getMeshes()) {
                if(std::get<1>(pair) == &mesh) {
                    const Shape& shape = *std::get<0>(pair);
                    std::vector<Vertex> vertices = mesh.getVertices();
                    const std::vector<Shape::Vertex>& shapeVertices = shape.getVertices();
                    for(const std::pair<int32_t, int32_t>& pair: shape.getMeshToShape()) {
                        vertices[pair.first].position_ = shapeVertices[pair.second].position_;
                        vertices[pair.first].normal_ = shapeVertices[pair.second].normal_;
                    }
                    saveMesh(mesh, vertices);
                } else {
                    saveMesh(mesh, mesh.getVertices());
                }
            }//for(const Mesh
        }//for(const Node
        file_.close();
    } // for(const std::tuple
}
} // namespace fbx2obj