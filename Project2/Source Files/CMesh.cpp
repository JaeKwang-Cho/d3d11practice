#include "pch.h"
#include "CMesh.h"

void CMesh::Initialize(const string _FilePath)
{
    m_RenderComp = new ObjectRenderComp;
    if (!LoadModelFromFile(_FilePath))
    {
        assert("LoadModelFromFile Failed" && false);
    }
}

void CMesh::StartObject()
{

}

void CMesh::UpdateObject()
{
    SetWorldMat();
}

void CMesh::RenderObject()
{
    auto iter = m_Meshes.begin();
    for (; iter != m_Meshes.end(); iter++)
    {
        (*iter)->StartRender();
        (*iter)->Render(m_WorldMat);
    }
}

bool CMesh::LoadModelFromFile(const string _FilePath)
{
    // #1 임포터를 만든다.
    Assimp::Importer importer;

    // #2 임포트 된 정보를 담는 aiScene 구조체를 초기화 한다.
    const aiScene* pScene = importer.ReadFile(_FilePath,
                                              aiProcess_Triangulate | // 3d로 가져오기
                                              aiProcess_ConvertToLeftHanded); // 왼손좌표계로 가져오기

    if (pScene == nullptr)
    {
        return false;
    }
    // #3 Scene 하위에 있는 Node를 처리한다.  
    ProcessNodes(pScene->mRootNode, pScene);
    return true;
}

void CMesh::ProcessNodes(aiNode* node, const aiScene* scene)
{
    // Node 안에는 또 다른 노드가 있을 수 있고, Mesh 가 있을 수 있다.
    // 메쉬 처리
    for (UINT i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.push_back(this->ProcessMesh(mesh, scene));
    }
    // 하위 노드 처리
    for (UINT i = 0; i < node->mNumChildren; i++)
    {
        ProcessNodes(node->mChildren[i], scene);
    }
}

MeshComp* CMesh::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    MeshComp* meshComp = nullptr;
    // 메쉬 데이터 채우기
    vector<DefaultVertex> vertices;
    vector<WORD>    indices;
    
    // 버텍스 가져오기
    vertices.reserve(mesh->mNumVertices);
    for (UINT i = 0; i < mesh->mNumVertices; i++)
    {
        DefaultVertex vertex;
        vertex.Pos.x = mesh->mVertices[i].x;
        vertex.Pos.y = mesh->mVertices[i].y;
        vertex.Pos.z = mesh->mVertices[i].z;

        // UV 정보가 있다면 가져오기
        if (mesh->mTextureCoords[0]) //0 ~ 7까지 존재한다.
        {
            vertex.Tex.u = (float)mesh->mTextureCoords[0][i].x;
            vertex.Tex.v = (float)mesh->mTextureCoords[0][i].y;
        }
        vertices.push_back(vertex);
    }

    // 인덱스 가져오기
    // Face 별로 가져오는데, 이렇게 하는 것은 위에서 aiProcess_Triangulate 플래그를 쓰면 이렇게 나온다.
    UINT numOfIndices = 0;
    for (UINT i = 0; i < mesh->mNumFaces; i++)
    {
        numOfIndices += mesh->mFaces[i].mNumIndices;
    }

    indices.reserve(numOfIndices);
    for (UINT i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (UINT j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    meshComp = new MeshComp;
    meshComp->Initialize(m_RenderComp, vertices, indices);
    return meshComp;
}

CObject* CMesh::Clone()
{
    return new CMesh(*this);
}


CMesh::CMesh()
    : m_RenderComp(nullptr)
    , m_Meshes()
{
}

CMesh::CMesh(const CMesh& _other)
	:CRenderAsset(_other)
    , m_RenderComp(nullptr)
    , m_Meshes()
{
}

CMesh::~CMesh()
{
    if (m_RenderComp)
    {
        delete m_RenderComp;
        m_RenderComp = nullptr;
    }

    auto iter = m_Meshes.begin();
    for (; iter != m_Meshes.end();)
    {
        MeshComp* p = *iter;
        delete p;
        iter = m_Meshes.erase(iter);
    }
}
