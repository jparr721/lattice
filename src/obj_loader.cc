#include <lattice/obj_loader.h>

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

bool LoadObjFile(const std::string& obj_file_path,
                 std::vector<Eigen::Vector3f>& vertices) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(obj_file_path, 0);

    if (!scene) {
        std::cerr << "ERROR: " << importer.GetErrorString() << std::endl;
        return false;
    }

    const aiMesh* mesh = scene->mMeshes[0];

    vertices.reserve(mesh->mNumVertices);
    for (auto i = 0u; i < mesh->mNumVertices; ++i) {
        aiVector3D pos = mesh->mVertices[i];
        vertices.emplace_back(pos.x, pos.y, pos.z);
    }

    return true;
}
