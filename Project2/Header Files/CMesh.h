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

public:
    void Initialize(const string _FilePath);
    virtual void StartObject() override;

    virtual void UpdateObject() override;
    virtual void RenderObject() override;

public:
    bool LoadModelFromFile(const string _FilePath);
    void ProcessNodes(aiNode* _node, const aiScene* _scene);
    MeshComp* ProcessMesh(aiMesh* _mesh, const aiScene* _scene);
    vector<TextureComp> LoadMaterialTexture(aiMaterial* _pMaterial, aiTextureType _textureType, const aiScene* _scene);

    virtual CObject* Clone() override;

public:
    CMesh();
    CMesh(const CMesh& _other);
    virtual ~CMesh();
};

