#pragma once
#include "CObject3D.h"
#include "MeshComp.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class CMesh :
    public CObject3D
{
private:
    vector<MeshComp*> m_Meshes;
    vector<MeshComp*> m_NonAlphaMeshes;
    vector<MeshComp*> m_AlphaMeshes;
    string m_directory;

protected:
    Matrix m_WorldMat;
    Matrix m_RenderMat;

public:
    void Initialize(const string _FilePath);
    virtual void StartObject() override;

    virtual void UpdateObject() override;

public:
    bool LoadModelFromFile(const string _FilePath);
    void ProcessNodes(aiNode* _node, const aiScene* _pScene, Matrix _ParentTransfromMat);
    MeshComp* ProcessMesh(aiMesh* _mesh, const aiScene* _pScene, Matrix _TransfromMat);
    TextureStorageType CheckTextureStorageType(const aiScene* _pScene, aiMaterial* _pMat, unsigned int _index, aiTextureType _textureType);
    vector<TextureComp> LoadMaterialTexture(aiMaterial* _pMaterial, aiTextureType _textureType, const aiScene* _pScene);

    virtual CObject* Clone() override;

protected:
    void UpdateWorldMat();
    void UpdateRenderMat();

public:
    CMesh();
    CMesh(const CMesh& _other);
    virtual ~CMesh();
};

