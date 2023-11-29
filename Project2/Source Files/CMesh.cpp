#include "pch.h"
#include "CMesh.h"

void CMesh::Initialize(const string _FilePath)
{
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

void CMesh::ProcessNodes(aiNode* _node, const aiScene* _scene)
{
    // Node 안에는 또 다른 노드가 있을 수 있고, Mesh 가 있을 수 있다.
    // 메쉬 처리
    for (UINT i = 0; i < _node->mNumMeshes; i++)
    {
        aiMesh* mesh = _scene->mMeshes[_node->mMeshes[i]];
        m_Meshes.push_back(ProcessMesh(mesh, _scene));
    }
    // 하위 노드 처리
    for (UINT i = 0; i < _node->mNumChildren; i++)
    {
        ProcessNodes(_node->mChildren[i], _scene);
    }
}

MeshComp* CMesh::ProcessMesh(aiMesh* _mesh, const aiScene* _scene)
{
    MeshComp* meshComp = nullptr;
    // 메쉬 데이터 채우기
    vector<DefaultVertex> vertices;
    vector<WORD>    indices;
    
    // 버텍스 가져오기
    vertices.reserve(_mesh->mNumVertices);
    for (UINT i = 0; i < _mesh->mNumVertices; i++)
    {
        DefaultVertex vertex;
        vertex.Pos.x = _mesh->mVertices[i].x;
        vertex.Pos.y = _mesh->mVertices[i].y;
        vertex.Pos.z = _mesh->mVertices[i].z;

        // UV 정보가 있다면 가져오기
        if (_mesh->mTextureCoords[0]) //0 ~ 7까지 존재한다.
        {
            vertex.Tex.u = (float)_mesh->mTextureCoords[0][i].x;
            vertex.Tex.v = (float)_mesh->mTextureCoords[0][i].y;
        }
        vertices.push_back(vertex);
    }

    // 인덱스 가져오기
    // Face 별로 가져오는데, 이렇게 하는 것은 위에서 aiProcess_Triangulate 플래그를 쓰면 이렇게 나온다.
    UINT numOfIndices = 0;
    for (UINT i = 0; i < _mesh->mNumFaces; i++)
    {
        numOfIndices += _mesh->mFaces[i].mNumIndices;
    }

    indices.reserve(numOfIndices);
    for (UINT i = 0; i < _mesh->mNumFaces; i++)
    {
        aiFace face = _mesh->mFaces[i];
        for (UINT j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    aiMaterial* material = _scene->mMaterials[_mesh->mMaterialIndex];
    vector<TextureComp> diffuseTextures = LoadMaterialTexture(material, aiTextureType::aiTextureType_DIFFUSE, _scene);

    meshComp = new MeshComp;
    meshComp->Initialize(vertices, indices, diffuseTextures);
    return meshComp;
}

vector<TextureComp> CMesh::LoadMaterialTexture(aiMaterial* _pMaterial, aiTextureType _textureType, const aiScene* _scene)
{
    vector<TextureComp> matTextures;
    TextureStorageType storeType = TextureStorageType::Invalid;
    unsigned int textureCount = _pMaterial->GetTextureCount(_textureType);

    TextureComp texComp;

    if (textureCount == 0) // 텍스쳐가 없을 때를 대비
    {
        storeType = TextureStorageType::None;
        aiColor3D aiColor(0.f, 0.f, 0.f);
        switch (_textureType)
        {
        case aiTextureType_DIFFUSE:
        {
            _pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
            if (aiColor.IsBlack()) //
            {
                texComp.CreateTextureResourceViewSimpleColor(DefaultColors::UnloadedTextureColor, _textureType);
                matTextures.push_back(texComp);

                return matTextures;
            }
            else
            {
                ColorComp diffuseColor(unsigned char(aiColor.r * 255.f), unsigned char(aiColor.g * 255.f), unsigned char(aiColor.b * 255));
                texComp.CreateTextureResourceViewSimpleColor(diffuseColor, _textureType);
                matTextures.push_back(texComp);

                return matTextures;
            }
        }

        break;
        default:
            break;
        }
    }
    texComp.CreateTextureResourceViewSimpleColor(DefaultColors::UnhandledTextureColor, aiTextureType_DIFFUSE);
    matTextures.push_back(texComp);

    return matTextures;
}

CObject* CMesh::Clone()
{
    return new CMesh(*this);
}


CMesh::CMesh()
    : m_Meshes()
{
}

CMesh::CMesh(const CMesh& _other)
	:CRenderAsset(_other)
    , m_Meshes()
{
}

CMesh::~CMesh()
{
    auto iter = m_Meshes.begin();
    for (; iter != m_Meshes.end();)
    {
        MeshComp* p = *iter;
        delete p;
        iter = m_Meshes.erase(iter);
    }
}
