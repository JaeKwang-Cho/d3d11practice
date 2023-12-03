#pragma once
#include "CRenderAsset.h"
#include "MeshComp.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>


class CMesh :
    public CRenderAsset
{
private:
    vector<MeshComp*> m_Meshes;
    vector<MeshComp*> m_NonAlphaMeshes;
    vector<MeshComp*> m_AlphaMeshes;
    string m_directory;

public:
    void Initialize(const string _FilePath);
    virtual void StartObject() override;

    virtual void UpdateObject() override;
    virtual void RenderObject() override;

public:
    bool LoadModelFromFile(const string _FilePath);
    void ProcessNodes(aiNode* _node, const aiScene* _pScene);
    MeshComp* ProcessMesh(aiMesh* _mesh, const aiScene* _pScene);
    TextureStorageType CheckTextureStorageType(const aiScene* _pScene, aiMaterial* _pMat, unsigned int _index, aiTextureType _textureType);
    vector<TextureComp> LoadMaterialTexture(aiMaterial* _pMaterial, aiTextureType _textureType, const aiScene* _pScene);

    virtual CObject* Clone() override;

public:
    CMesh();
    CMesh(const CMesh& _other);
    virtual ~CMesh();
};

