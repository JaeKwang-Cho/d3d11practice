#include "pch.h"
#include "CMesh.h"

void CMesh::Initialize(const string _FilePath)
{
    m_directory = StringHelper::GetDirectoryFromPath(_FilePath);
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
    UpdateRenderMat();

    MVPMatrix cbMVPMatrix;
    cbMVPMatrix.mat = MatrixTranspose(m_RenderMat);
    g_pImmediateContext->UpdateSubresource(g_pCBMVPMat, 0, nullptr, &cbMVPMatrix, 0, 0);
    // �� ������ ���� ���ϴ� constant buffer ������Ʈ �ϱ�
}

void CMesh::RenderObject()
{
    g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // ����� ������ �ε����� ���� �ﰢ���� �׸����� ��Ų��.
    g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pCBMVPMat);

    m_Meshes[1]->StartRender();
    m_Meshes[1]->Render();
    m_Meshes[2]->StartRender();
    m_Meshes[2]->Render();
    m_Meshes[3]->StartRender();
    m_Meshes[3]->Render();
    m_Meshes[0]->StartRender();
    m_Meshes[0]->Render();
    
    /*
    auto iter = m_Meshes.begin();
    for (; iter != m_Meshes.end(); iter++)
    {
        (*iter)->StartRender();
        (*iter)->Render();
    }
    */
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

#if 0
    {
        aiMaterial* mat1 = pScene->mMaterials[0];

        int countNone = mat1->GetTextureCount(aiTextureType_NONE);
        int countDiffuse = mat1->GetTextureCount(aiTextureType_DIFFUSE);
        int countSpecular = mat1->GetTextureCount(aiTextureType_SPECULAR);
        int countAmbient = mat1->GetTextureCount(aiTextureType_AMBIENT);
        int countEmissive = mat1->GetTextureCount(aiTextureType_EMISSIVE);
        int countHeight = mat1->GetTextureCount(aiTextureType_HEIGHT);
        int countNomals = mat1->GetTextureCount(aiTextureType_NORMALS);
        int countShininess = mat1->GetTextureCount(aiTextureType_SHININESS);
        int countOpacity = mat1->GetTextureCount(aiTextureType_OPACITY);
        int countDisplacement = mat1->GetTextureCount(aiTextureType_DISPLACEMENT);
        int countLightMap = mat1->GetTextureCount(aiTextureType_LIGHTMAP);
        int countReflection = mat1->GetTextureCount(aiTextureType_REFLECTION);
        int countBaseColor = mat1->GetTextureCount(aiTextureType_BASE_COLOR);
        int countNormalCamera = mat1->GetTextureCount(aiTextureType_NORMAL_CAMERA);
        int countEmissionColor = mat1->GetTextureCount(aiTextureType_EMISSION_COLOR);
        int countMetalness = mat1->GetTextureCount(aiTextureType_METALNESS);
        int countDiffuseRoughness = mat1->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS);
        int countRoughness = mat1->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION);
        int countSheen = mat1->GetTextureCount(aiTextureType_SHEEN);
        int countClearCoat = mat1->GetTextureCount(aiTextureType_CLEARCOAT);
        int countTransmission = mat1->GetTextureCount(aiTextureType_TRANSMISSION);
        int countUnKnown = mat1->GetTextureCount(aiTextureType_UNKNOWN);
    }
#endif
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

MeshComp* CMesh::ProcessMesh(aiMesh* _mesh, const aiScene* _pScene)
{
    MeshComp* meshComp = nullptr;
    // �޽� ������ ä���
    vector<DefaultVertex> vertices;
    vector<WORD>    indices;
    
    // ���ؽ� ��������
    vertices.reserve(_mesh->mNumVertices);
    //wchar_t szBuffer[255] = {};
    aiMaterial* mat = _pScene->mMaterials[_mesh->mMaterialIndex];

    bool bAlphaLessOne = false;
    ai_real alphaVal = 1.f;
    if (mat->Get(AI_MATKEY_OPACITY, alphaVal) == AI_SUCCESS)
    {
        if (alphaVal < 1.f)
        {
            bAlphaLessOne = true;
        }
    }

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
            
            //swprintf_s(szBuffer, L"(u, v) = (%.2f, %.2f)\n", vertex.Tex.u, vertex.Tex.v);
            //OutputDebugStringW(szBuffer);
        }

        vertex.Alpha = alphaVal;
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
    aiMaterial* material = _pScene->mMaterials[_mesh->mMaterialIndex];
    vector<TextureComp> diffuseTextures = LoadMaterialTexture(material, aiTextureType::aiTextureType_DIFFUSE, _pScene);

    meshComp = new MeshComp;
    meshComp->Initialize(vertices, indices, diffuseTextures, bAlphaLessOne);
    return meshComp;
}

TextureStorageType CMesh::CheckTextureStorageType(const aiScene* _pScene, aiMaterial* _pMat, unsigned int _index, aiTextureType _textureType)
{
    // �ؽ��İ� ���� ��
    if (_pMat->GetTextureCount(_textureType) == 0)
    {
        return TextureStorageType::None;
    }

    aiString path;
    _pMat->GetTexture(_textureType, _index, &path);
    string texturePath = path.C_Str();
    // ����� �ؽ��İ� �ְ� �ε��̵� �Ǿ� �ִ�.
    // (�ƽ��͸���ũ�� ���� �� ���̴�.)
    if (texturePath[0] == '*')
    {
        if (_pScene->mTextures[0]->mHeight == 0)
        {
            return TextureStorageType::EmbeddedIndexCompressed;
        }
        else
        {
            assert("SUPPORT DOES NOT EXIST YET FOR INDEXED NON COMPRESSED TEXTURES!" && 0);
            return TextureStorageType::EmbeddedIndexNonCompressed;
        }
    }
    // ����� �ؽ��İ� ������, �ε��� �Ǿ����� ���� ��� (��ΰ� # ��ſ� �ؽ��� �̸��� �ȴ�.)
    if (auto pTex = _pScene->GetEmbeddedTexture(texturePath.c_str()))
    {
        if (pTex->mHeight == 0)
        {
            return TextureStorageType::EmbeddedCompressed;
        }
        else
        {
            assert("SUPPORT DOES NOT EXIST YET FOR EMBEDDED NON COMPRESSED TEXTURES!" && 0);
            return TextureStorageType::EmbeddedNonCompressed;
        }
    }
    
    // ���������� �ؽ��İ� ���� ��ο� �ִ��� Ȯ���Ѵ�.
    if (texturePath.find('.') != string::npos)
    {
        return TextureStorageType::Disk;
    }
    // �ؽ��İ� ���� ��
    return TextureStorageType::None;
}

vector<TextureComp> CMesh::LoadMaterialTexture(aiMaterial* _pMaterial, aiTextureType _textureType, const aiScene* _pScene)
{
    vector<TextureComp> matTextures;
    TextureStorageType storetype = TextureStorageType::Invalid;
    unsigned int textureCount = _pMaterial->GetTextureCount(_textureType);

    TextureComp texComp;

    if (textureCount == 0) // �ؽ��İ� ���� ���� ���
    {
        storetype = TextureStorageType::None;
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
    else
    {
        for (UINT i = 0; i < textureCount; i++)
        {
            aiString path;
            _pMaterial->GetTexture(_textureType, i, &path);
            storetype = CheckTextureStorageType(_pScene, _pMaterial, i, _textureType);
            switch (storetype)
            {
            case TextureStorageType::Disk:
            {
                string filename = m_directory + '\\' + path.C_Str();
                TextureComp diskTexture;
                diskTexture.CreateTextureResourceViewFromImage(filename, _textureType);
                matTextures.push_back(diskTexture);
            }
            break;
            case TextureStorageType::EmbeddedCompressed:
            {
                int i = 0;
            }
            break;
            case TextureStorageType::EmbeddedNonCompressed:
            {
                int i = 0;
            }
            break;
            case TextureStorageType::EmbeddedIndexCompressed:
            {
                int i = 0;
            }
            break;
            case TextureStorageType::EmbeddedIndexNonCompressed:
            {
                int i = 0;
            }
            break;
            default:
                int i = 0;
                break;
            }
        }
    }

    if (matTextures.size() == 0)
    {
        texComp.CreateTextureResourceViewSimpleColor(DefaultColors::UnhandledTextureColor, aiTextureType_DIFFUSE);
        matTextures.push_back(texComp);
    }
    

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
