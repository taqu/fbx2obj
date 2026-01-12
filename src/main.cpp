#include <filesystem>
#include "Scene.h"
#include "Exporter.h"

int main(int argc, char** argv)
{
    if(argc<=1) {
        return 0;
    }

    fbx2obj::Scene scene = fbx2obj::Scene::load(argv[1]);
    {
    fbx2obj::Exporter exporter;
    std::filesystem::path path(argv[1]);
    path = path.replace_extension("obj");
    exporter.save(path.string().c_str(), scene);
    }
    return 0;
}
