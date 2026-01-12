#include <filesystem>
#include "Scene.h"
#include "Exporter.h"

int main(int argc, char** argv)
{
    #if 1
    {
        const char* model = "data/SpiderThomas/SpiderThomas.fbx";
        fbx2obj::Scene scene = fbx2obj::Scene::load(model);
        {
            fbx2obj::Exporter exporter;
            std::filesystem::path path(model);
            path = path.replace_extension("obj");
            exporter.save(path.string().c_str(), scene);
        }
    }
    {
        const char* model = "data/lan-laffinty/source/Lan Laffinty.fbx";
        fbx2obj::Scene scene = fbx2obj::Scene::load(model);
        {
            fbx2obj::Exporter exporter;
            std::filesystem::path path(model);
            path = path.replace_extension("obj");
            exporter.save(path.string().c_str(), scene);
        }
    }
    #endif
    {
        const char* model = "data/Cecilia/Cecilia 4.fbx";
        fbx2obj::Scene scene = fbx2obj::Scene::load(model);
        {
            fbx2obj::Exporter exporter;
            std::filesystem::path path(model);
            path = path.replace_extension("obj");
            exporter.save(path.string().c_str(), scene);
        }
    }
    return 0;
}
