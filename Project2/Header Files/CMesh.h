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
    void ProcessNodes(aiNode* node, const aiScene* scene);
    MeshComp* ProcessMesh(aiMesh* mesh, const aiScene* scene);

    virtual CObject* Clone() override;

public:
    CMesh();
    CMesh(const CMesh& _other);
    virtual ~CMesh();
};

