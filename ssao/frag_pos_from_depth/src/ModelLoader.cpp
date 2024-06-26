#include "ModelLoader.hpp"

#include <iostream>
#include "Scene.hpp"
#include "ModelNode.hpp"
#include "ModelDrawable.hpp"
#include "FileSystem.hpp"
#include "TextureManager.hpp"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <glm/glm.hpp>
#include <random>

namespace
{
    std::vector<std::string> LoadTextures(aiMaterial* mat, aiTextureType type, const std::string& directory, unsigned internalFormat)
    {
        std::vector<std::string> names;
        for(unsigned i = 0; i < mat->GetTextureCount(type); ++i)
        {
            aiString path;
            mat->GetTexture(type, i, &path);
            std::string fullPath = directory + path.C_Str();
            TextureManager::instance().LoadTexture(fullPath, fullPath, internalFormat);
            names.push_back(fullPath);
        }

        return names;
    }

    ModelNode traverseNode(const aiNode* node, int depth)
    {
        std::vector<unsigned> meshIndices;
        for(unsigned i = 0; i < node->mNumMeshes; ++i)
        {
            meshIndices.push_back(node->mMeshes[i]);
        }

        glm::mat4 transformation;
        auto nodeTransformation = node->mTransformation[0];
        
        for(unsigned row = 0; row < 4; ++row)
        {
            for(unsigned col = 0; col < 4; ++col)
            {
                transformation[col][row] = nodeTransformation[row * 4 + col];
            }
        }

        ModelNode modelNode(node->mName.C_Str(), meshIndices, transformation);

        for (unsigned i = 0; i < node->mNumChildren; ++i)
        {
            modelNode.AddChild(traverseNode(node->mChildren[i], depth + 1));
        }

        return modelNode;
    }

    ModelNode traverseScene(const aiScene* scene, std::vector<Mesh>& modelMeshes, const std::string& directory)
    {
        for (unsigned i = 0; i < scene->mNumMeshes; ++i)
        {
            std::vector<tangent_vertex> vertices;
            auto mesh = scene->mMeshes[i];
            for (unsigned vIndex = 0; vIndex < mesh->mNumVertices; ++vIndex)
            {
                auto v = mesh->mVertices[vIndex];
                auto t = mesh->mTangents[vIndex];
                auto b = mesh->mBitangents[vIndex];
                auto n = mesh->mNormals[vIndex];

                tangent_vertex vert{{ v.x, v.y, v.z }, { 0.f, 0.0f, 0.f}, { 0.f, 0.f }, { n.x, n.y, n.z }, { t.x, t.y, t.z }, { b.x, b.y, b.z }};

                if (mesh->mTextureCoords[0])
                {
                    auto texCoords = mesh->mTextureCoords[0][vIndex];
                    vert.tex_coords.s = texCoords.x;
                    vert.tex_coords.t = texCoords.y;
                }

                vertices.push_back(vert);
            }

            std::vector<unsigned> indices;
            for (unsigned fIndex = 0; fIndex < mesh->mNumFaces; ++fIndex)
            {
                auto f = mesh->mFaces[fIndex];
                indices.push_back(f.mIndices[0]);
                indices.push_back(f.mIndices[1]);
                indices.push_back(f.mIndices[2]);
            }

            std::vector<std::string> diffuseTextures;
            std::vector<std::string> specularTextures;
            std::vector<std::string> normalmapTextures;


            if (mesh->mMaterialIndex >= 0)
            {
                auto material = scene->mMaterials[mesh->mMaterialIndex];
                diffuseTextures = LoadTextures(material, aiTextureType_DIFFUSE, directory, GL_SRGB_ALPHA);
                specularTextures = LoadTextures(material, aiTextureType_SPECULAR, directory, GL_RGB);
                normalmapTextures = LoadTextures(material, aiTextureType_HEIGHT, directory, GL_RGBA);
            }

            modelMeshes.emplace_back(vertices, indices, diffuseTextures, specularTextures, normalmapTextures);
        }

        return traverseNode(scene->mRootNode, 0);
    }
}

std::unique_ptr<ModelDrawable> ModelLoader::Load(Scene& scene, const char* path)
{
    Assimp::Importer importer;
    std::string fullPath = FileSystem::ResourcesDirectory() + path;
    auto assimpScene = importer.ReadFile(fullPath.c_str()
        , aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!assimpScene)
    {
        return nullptr;
    }

    std::vector<Mesh> modelMeshes;

    auto directory  = FileSystem::getDirectory(fullPath);
    return std::make_unique<ModelDrawable>(scene, traverseScene(assimpScene, modelMeshes, directory)
                                            , std::move(modelMeshes));
}

