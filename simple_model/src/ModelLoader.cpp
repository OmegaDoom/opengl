#include "ModelLoader.hpp"

#include "Scene.hpp"
#include "ModelNode.hpp"
#include "ModelDrawable.hpp"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <glm/glm.hpp>

namespace
{
    ModelNode traverseNode(const aiNode* node, int depth)
    {
        std::vector<unsigned> meshIndices;
        for(int i = 0; i < node->mNumMeshes; ++i)
        {
            meshIndices.push_back(node->mMeshes[i]);
        }

        glm::mat4 transformation;
        auto nodeTransformation = node->mTransformation[0];
        
        for(int row = 0; row < 4; ++row)
        {
            for(int col = 0; col < 4; ++col)
            {
                transformation[col][row] = nodeTransformation[row * 4 + col];
            }
        }

        ModelNode modelNode(meshIndices, transformation);

        for (int i = 0; i < node->mNumChildren; ++i)
        {
            modelNode.AddChild(traverseNode(node->mChildren[i], depth + 1));
        }

        return modelNode;
    }

    ModelNode traverseScene(const aiScene* scene, std::vector<Mesh>& modelMeshes)
    {
        for (int i = 0; i < scene->mNumMeshes; ++i)
        {
            std::vector<vertex> vertices;
            auto mesh = scene->mMeshes[i];
            for (int vIndex = 0; vIndex < mesh->mNumVertices; ++vIndex)
            {
                auto v = mesh->mVertices[vIndex];
                auto n = mesh->mNormals[vIndex];

                vertex vert{{ v.x, v.y, v.z }, { 0.f, 0.0f, 0.f}, { 0.f, 0.f }, { n.x, n.y, n.z }}; 
                if (mesh->mTextureCoords[0])
                {
                    auto texCoords = mesh->mTextureCoords[0][vIndex];
                    vert.tex_coords.s = texCoords.x;
                    vert.tex_coords.t = texCoords.y;
                }
                vertices.push_back(vert);
            }

            std::vector<unsigned> indices;
            for (int fIndex = 0; fIndex < mesh->mNumFaces; ++fIndex)
            {
                auto f = mesh->mFaces[fIndex];
                indices.push_back(f.mIndices[0]);
                indices.push_back(f.mIndices[1]);
                indices.push_back(f.mIndices[2]);
            }

            modelMeshes.emplace_back(vertices, indices);
        }

        return traverseNode(scene->mRootNode, 0);
    }
}

std::unique_ptr<ModelDrawable> ModelLoader::Load(Scene& scene, const char* path)
{
    Assimp::Importer importer;
    std::string fullPath = "./resources/";
    fullPath += path;
    auto assimpScene = importer.ReadFile(fullPath.c_str()
        , aiProcess_Triangulate | aiProcess_GenSmoothNormals);

    if (!assimpScene)
    {
        return nullptr;
    }

    std::vector<Mesh> modelMeshes;

    return std::make_unique<ModelDrawable>(scene, traverseScene(assimpScene, modelMeshes), std::move(modelMeshes));
}

