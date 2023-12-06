#include "pch.h"
#include "CMesh.h"
#include "CRenderManager.h"

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
    // �� ������ ���� ���ϴ� constant buffer ������Ʈ �ϱ�
    auto iter = m_NonAlphaMeshes.begin();
    for (; iter != m_NonAlphaMeshes.end(); iter++)
    {
        (*iter)->UpdateComp(m_WorldMat);
    }

    CRenderManager::GetInstance()->EnqueNonAlphaMeshComps(&m_NonAlphaMeshes);

    iter = m_AlphaMeshes.begin();
    for (; iter != m_AlphaMeshes.end(); iter++)
    {
        MeshComp* pIter = *iter;
        pIter->UpdateComp(m_WorldMat);
        pIter->CalcZValue();
        CRenderManager::GetInstance()->EnqueAlphaMeshComp(pIter);
    }

    UpdateRenderMat();
}

void CMesh::RenderObject()
{
    auto iter = m_NonAlphaMeshes.begin();
    for (; iter != m_NonAlphaMeshes.end(); iter++)
    {
        (*iter)->RenderComp();
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
    ProcessNodes(pScene->mRootNode, pScene, MatrixIdentity());
    return true;
}

void CMesh::ProcessNodes(aiNode* _node, const aiScene* _scene, Matrix _ParentTransfromMat)
{
    Matrix nodeTransfromMat = MatrixTranspose(Matrix(&_node->mTransformation.a1)) * _ParentTransfromMat;
    // Node �ȿ��� �� �ٸ� ��尡 ���� �� �ְ�, Mesh �� ���� �� �ִ�.
    // �޽� ó��
    for (UINT i = 0; i < _node->mNumMeshes; i++)
    {
        aiMesh* mesh = _scene->mMeshes[_node->mMeshes[i]];
        MeshComp* pMeshComp = ProcessMesh(mesh, _scene, nodeTransfromMat);
        m_Meshes.push_back(pMeshComp);
        if (pMeshComp->m_bAlphaLessOne)
        {
            m_AlphaMeshes.push_back(pMeshComp);
        }
        else
        {
            m_NonAlphaMeshes.push_back(pMeshComp);
        }
    }
    // ���� ��� ó��
    for (UINT i = 0; i < _node->mNumChildren; i++)
    {
        ProcessNodes(_node->mChildren[i], _scene, nodeTransfromMat);
    }
}

MeshComp* CMesh::ProcessMesh(aiMesh* _mesh, const aiScene* _pScene, Matrix _TransfromMat)
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

        vertex.Normal.x = _mesh->mNormals[i].x;
        vertex.Normal.y = _mesh->mNormals[i].y;
        vertex.Normal.z = _mesh->mNormals[i].z;
        
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
    meshComp->Initialize(vertices, indices, diffuseTextures, bAlphaLessOne, _TransfromMat);

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
                TexResource::CreateTextureResourceViewSingleColor(&texComp.m_pTextureResourceView, DefaultColors::UnloadedTextureColor);
                texComp.m_type = _textureType;
                texComp.m_pTextureResourceView->GetResource(&texComp.m_texture);
                matTextures.push_back(texComp);

                return matTextures;
            }
            else
            {
                ColorComp diffuseColor(unsigned char(aiColor.r * 255.f), unsigned char(aiColor.g * 255.f), unsigned char(aiColor.b * 255));
                TexResource::CreateTextureResourceViewSingleColor(&texComp.m_pTextureResourceView, diffuseColor);
                texComp.m_type = _textureType;
                texComp.m_pTextureResourceView->GetResource(&texComp.m_texture);
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
                TexResource::CreateTextureResourceViewFromImage(&diskTexture.m_pTextureResourceView, StringHelper::StrToWstr(filename));
                diskTexture.m_type = _textureType;
                diskTexture.m_pTextureResourceView->GetResource(&diskTexture.m_texture);
                matTextures.push_back(diskTexture);
            }
            break;
            case TextureStorageType::EmbeddedCompressed:
            {
                const aiTexture* pTexture = _pScene->GetEmbeddedTexture(path.C_Str());
                TextureComp EmbeddedTexture;
                TexResource::CreateTextureResourceViewFromData(&EmbeddedTexture.m_pTextureResourceView, reinterpret_cast<uint8_t*>(pTexture->pcData), pTexture->mWidth);
                EmbeddedTexture.m_type = _textureType;
                EmbeddedTexture.m_pTextureResourceView->GetResource(&EmbeddedTexture.m_texture);
                matTextures.push_back(EmbeddedTexture);
            }
            break;
            case TextureStorageType::EmbeddedNonCompressed:
            {
                assert("TextureStorageType::EmbeddedNonCompressed isn't implemented" && false);
            }
            break;
            case TextureStorageType::EmbeddedIndexCompressed:
            {
                const char* data = path.C_Str();
                int index = atoi(data + 1);
                TextureComp EmbeddedIndexTexture;
                TexResource::CreateTextureResourceViewFromData(&EmbeddedIndexTexture.m_pTextureResourceView, reinterpret_cast<uint8_t*>(_pScene->mTextures[index]->pcData), _pScene->mTextures[index]->mWidth);
                EmbeddedIndexTexture.m_type = _textureType;
                EmbeddedIndexTexture.m_pTextureResourceView->GetResource(&EmbeddedIndexTexture.m_texture);
                matTextures.push_back(EmbeddedIndexTexture);
            }
            break;
            case TextureStorageType::EmbeddedIndexNonCompressed:
            {
                assert("TextureStorageType::EmbeddedIndexNonCompressed isn't implemented" && false);
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
        TexResource::CreateTextureResourceViewSingleColor(&texComp.m_pTextureResourceView, DefaultColors::UnhandledTextureColor);
        texComp.m_type = aiTextureType_DIFFUSE;
        texComp.m_pTextureResourceView->GetResource(&texComp.m_texture);
        matTextures.push_back(texComp);
    }
    

    return matTextures;
}

CObject* CMesh::Clone()
{
    return new CMesh(*this);
}

void CMesh::UpdateWorldMat()
{
    // ��� �� ������ ������ -> ���� -> ���� (�̵� + ȸ��) ������ ���ϸ� �ȴ�.
    m_WorldMat = MatrixIdentity();

    Matrix ScaleMat = MatrixScale(GetWorldTransform().Scale);

    Matrix RotMat = MatrixRotationZ(GetWorldTransform().Rotation.z);
    RotMat *= MatrixRotationX(GetWorldTransform().Rotation.x);
    RotMat *= MatrixRotationX(GetWorldTransform().Rotation.y);

    Matrix TransMat = MatrixTranslation(GetWorldTransform().Position);

    m_WorldMat = ScaleMat * RotMat * TransMat;
}

void CMesh::UpdateRenderMat()
{
    //1. RenderAsset���� ���� ������ �ִ� RenderManager
    //2. ���⼭ �ϴ�, �������� �ֵ��� ���� ����Ѵ�.
    //3. �׸��� alpha�� < 0 �� ģ������ sort �Ѵ�.
    //4. �׸��� ����Ѵ�.
    UpdateWorldMat();

    m_RenderMat = m_WorldMat * g_ViewMat * g_ProjectionMat;
}



CMesh::CMesh()
    : m_Meshes()
{
}

CMesh::CMesh(const CMesh& _other)
	:CObject(_other)
    , m_WorldMat(MatrixIdentity())
    , m_RenderMat(MatrixIdentity())
    , m_Meshes()
{
}

CMesh::~CMesh()
{
    auto iter = m_Meshes.begin();
    for (; iter != m_Meshes.end(); iter++)
    {
        MeshComp* p = *iter;
        delete p;
    }
    m_Meshes.clear();
}
