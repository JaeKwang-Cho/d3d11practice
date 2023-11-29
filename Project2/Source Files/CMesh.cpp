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
    // #1 �����͸� �����.
    Assimp::Importer importer;

    // #2 ����Ʈ �� ������ ��� aiScene ����ü�� �ʱ�ȭ �Ѵ�.
    const aiScene* pScene = importer.ReadFile(_FilePath,
                                              aiProcess_Triangulate | // 3d�� ��������
                                              aiProcess_ConvertToLeftHanded); // �޼���ǥ��� ��������

    if (pScene == nullptr)
    {
        return false;
    }
    // #3 Scene ������ �ִ� Node�� ó���Ѵ�.  
    ProcessNodes(pScene->mRootNode, pScene);
    return true;
}

void CMesh::ProcessNodes(aiNode* _node, const aiScene* _scene)
{
    // Node �ȿ��� �� �ٸ� ��尡 ���� �� �ְ�, Mesh �� ���� �� �ִ�.
    // �޽� ó��
    for (UINT i = 0; i < _node->mNumMeshes; i++)
    {
        aiMesh* mesh = _scene->mMeshes[_node->mMeshes[i]];
        m_Meshes.push_back(ProcessMesh(mesh, _scene));
    }
    // ���� ��� ó��
    for (UINT i = 0; i < _node->mNumChildren; i++)
    {
        ProcessNodes(_node->mChildren[i], _scene);
    }
}

MeshComp* CMesh::ProcessMesh(aiMesh* _mesh, const aiScene* _scene)
{
    MeshComp* meshComp = nullptr;
    // �޽� ������ ä���
    vector<DefaultVertex> vertices;
    vector<WORD>    indices;
    
    // ���ؽ� ��������
    vertices.reserve(_mesh->mNumVertices);
    for (UINT i = 0; i < _mesh->mNumVertices; i++)
    {
        DefaultVertex vertex;
        vertex.Pos.x = _mesh->mVertices[i].x;
        vertex.Pos.y = _mesh->mVertices[i].y;
        vertex.Pos.z = _mesh->mVertices[i].z;

        // UV ������ �ִٸ� ��������
        if (_mesh->mTextureCoords[0]) //0 ~ 7���� �����Ѵ�.
        {
            vertex.Tex.u = (float)_mesh->mTextureCoords[0][i].x;
            vertex.Tex.v = (float)_mesh->mTextureCoords[0][i].y;
        }
        vertices.push_back(vertex);
    }

    // �ε��� ��������
    // Face ���� �������µ�, �̷��� �ϴ� ���� ������ aiProcess_Triangulate �÷��׸� ���� �̷��� ���´�.
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

    if (textureCount == 0) // �ؽ��İ� ���� ���� ���
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
