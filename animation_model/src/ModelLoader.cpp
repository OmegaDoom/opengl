#include "ModelLoader.hpp"

#include <iostream>
#include "Scene.hpp"
#include "Bone.hpp"
#include "VertexWeight.hpp"
#include "ModelNode.hpp"
#include "ModelDrawable.hpp"
#include "FileSystem.hpp"
#include "TextureManager.hpp"
#include "ModelNodeAnimation.hpp"
#include "Animation.hpp"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <glm/glm.hpp>
#include <random>

namespace
{
    std::vector<std::string> LoadTextures(aiMaterial* mat, aiTextureType type, const std::string& directory)
    {
        std::vector<std::string> names;
        for(unsigned i = 0; i < mat->GetTextureCount(type); ++i)
        {
            aiString path;
            mat->GetTexture(type, i, &path);
            std::string fullPath = directory + path.C_Str();
            TextureManager::instance().LoadTexture(fullPath, fullPath);
            names.push_back(fullPath);
        }

        return names;
    }

    ModelNodeAnimation GetModelNodeAnimation(aiNodeAnim* nodeAnim)
    {
        ModelNodeAnimation modelNodeAnimation{ nodeAnim->mNodeName.C_Str()};

        for (unsigned posKeyIndex = 0; posKeyIndex < nodeAnim->mNumPositionKeys; ++posKeyIndex)
        {
            auto positionKey = nodeAnim->mPositionKeys[posKeyIndex];
            modelNodeAnimation.AddPositionKey(AnimationPositionKey(positionKey.mTime
                        , glm::vec3(positionKey.mValue.x
                            , positionKey.mValue.y
                            , positionKey.mValue.z)));
        }

        for (unsigned rotKeyIndex = 0; rotKeyIndex < nodeAnim->mNumRotationKeys; ++rotKeyIndex)
        {
            auto rotationKey = nodeAnim->mRotationKeys[rotKeyIndex];
            modelNodeAnimation.AddRotationKey(AnimationRotationKey(rotationKey.mTime
                        , glm::quat(rotationKey.mValue.w
                            , rotationKey.mValue.x
                            , rotationKey.mValue.y
                            , rotationKey.mValue.z)));
        }

        for (unsigned scalingKeyIndex = 0; scalingKeyIndex < nodeAnim->mNumScalingKeys; ++scalingKeyIndex)
        {
            auto scalingKey = nodeAnim->mScalingKeys[scalingKeyIndex];
            modelNodeAnimation.AddScaleKey(AnimationScaleKey(scalingKey.mTime
                        , glm::vec3(scalingKey.mValue.x
                            , scalingKey.mValue.y
                            , scalingKey.mValue.z)));
        }

        return modelNodeAnimation;
    }

    glm::mat4 Convert(const aiMatrix4x4& aiTransform)
    {
        glm::mat4 transformation;
        for(int row = 0; row < 4; ++row)
        {
            for(int col = 0; col < 4; ++col)
            {
                transformation[col][row] = aiTransform[row][col];
            }
        }
        
        return transformation;
    }

    ModelNode traverseNode(const aiNode* node, int depth)
    {
        std::vector<unsigned> meshIndices;
        for(unsigned i = 0; i < node->mNumMeshes; ++i)
        {
            meshIndices.push_back(node->mMeshes[i]);
        }

        auto transformation = Convert(node->mTransformation);

        ModelNode modelNode(node->mName.C_Str(), meshIndices, transformation);

        for (unsigned i=0; i < node->mNumChildren; ++i)
        {
            modelNode.AddChild(traverseNode(node->mChildren[i], depth + 1));
        }

        return modelNode;
    }

    ModelNode traverseScene(const aiScene* scene, std::vector<Mesh>& modelMeshes
                            , std::vector<Animation>& animations
                            , const std::string& directory)
    {
        for (unsigned meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
        {
            std::vector<vertex> vertices;
            auto mesh = scene->mMeshes[meshIndex];
            for (unsigned vIndex = 0; vIndex < mesh->mNumVertices; ++vIndex)
            {
                auto v = mesh->mVertices[vIndex];
                auto n = mesh->mNormals[vIndex];

                //vertex vert{{ v.x, v.y, v.z }, { 0.f, 0.0f, 0.f}, { 0.f, 0.f }, { n.x, n.y, n.z }
                //            , {{-1, -1, -1, -1}, {0.f, 0.f, 0.f, 0.f}}}; 

                vertex vert{{ v.x, v.y, v.z }, { 0.f, 0.0f, 0.f}, { 0.f, 0.f }, { n.x, n.y, n.z }
                            , {{-1, -1, -1, -1}, {0.f, 0.f, 0.f, 0.f}}}; 
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

            if (mesh->mMaterialIndex >= 0)
            {
                auto material = scene->mMaterials[mesh->mMaterialIndex];
                diffuseTextures = LoadTextures(material, aiTextureType_DIFFUSE, directory);
                specularTextures = LoadTextures(material, aiTextureType_SPECULAR, directory);
            }

            std::vector<Bone> bones;
            if (mesh->HasBones())
            {
                for (unsigned boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
                {
                    auto bone = mesh->mBones[boneIndex];

                    std::vector<VertexWeight> weights;
                    for(unsigned weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex)
                    {
                        auto weight = bone->mWeights[weightIndex];
                        weights.emplace_back(weight.mVertexId, weight.mWeight);
                        
                        for(int vertexWeightIndex = 0; vertexWeightIndex < 4; ++vertexWeightIndex)
                        {
                            if (-1 == vertices[weight.mVertexId].bones_info.ids[vertexWeightIndex]) 
                            {
                                vertices[weight.mVertexId].bones_info.ids[vertexWeightIndex] = boneIndex;
                                vertices[weight.mVertexId].bones_info.weights[vertexWeightIndex] = weight.mWeight;
                                break;
                            }
                        }
                    }

                    bones.push_back(Bone(bone->mName.C_Str(), boneIndex, weights, Convert(bone->mOffsetMatrix)));
                }
            }

            modelMeshes.emplace_back(vertices, indices, diffuseTextures, specularTextures, bones);
        }

        if (scene->HasAnimations())
        {
            for (unsigned i = 0; i < scene->mNumAnimations; ++i)
            {
                auto animation = scene->mAnimations[i];
                double ticksPerSecond = animation->mTicksPerSecond > 0.0 ? animation->mTicksPerSecond : 1000.0;
                auto duration = animation->mDuration / ticksPerSecond;

                Animation modelAnimation(animation->mName.C_Str(), duration);
                for (unsigned animChannel = 0; animChannel < animation->mNumChannels; ++animChannel)
                {
                    auto nodeAnim = animation->mChannels[animChannel];
                    modelAnimation.AddModelNodeAnimation(GetModelNodeAnimation(nodeAnim));
                }

                animations.push_back(modelAnimation);
            }
        }

        return traverseNode(scene->mRootNode, 0);
    }
}

std::unique_ptr<ModelDrawable> ModelLoader::Load(Scene& scene, const char* path)
{
    Assimp::Importer importer;
    std::string fullPath = FileSystem::ResourcesDirectory() + path;
    auto assimpScene = importer.ReadFile(fullPath.c_str()
            , aiProcess_Triangulate
            | aiProcess_GenNormals);

    if (!assimpScene)
    {
        return nullptr;
    }

    std::vector<Mesh> modelMeshes;
    std::vector<Animation> animations;

    auto directory  = FileSystem::getDirectory(fullPath);
    return std::make_unique<ModelDrawable>(scene, traverseScene(assimpScene, modelMeshes, animations, directory)
            , std::move(modelMeshes), animations);
}

