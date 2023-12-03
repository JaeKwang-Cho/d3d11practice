#pragma once 
#include "MeshComp.h"

struct MeshComp;

struct ZComparer {
	bool operator()(MeshComp* _lh, MeshComp* _rh)
	{
		if (_lh->m_fZval > _rh->m_fZval)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

/*
template<typename Type, typename Compare = std::less<Type> >
struct ComparerTemplate : public std::binary_function<Type*, Type*, bool> {
	bool operator()(const Type* x, const Type* y) const
	{
		return Compare()(x, y);
	}
};
*/

class CRenderManager
{
	SINGLETON(CRenderManager)

public:
	void EnqueAlphaMeshComp(MeshComp* _alphaMesh);
	void EnqueNonAlphaMeshComps(vector<MeshComp*>* _nonAlphaMeshes);

private:
	priority_queue<MeshComp*, vector<MeshComp*>, ZComparer> m_AlphaMeshes;
	vector<vector<MeshComp*>*> m_NonAlphaMeshes;

private:
	ID3D11SamplerState* m_TextureSampler;

	HRESULT CreateDefaultTextureSampler();
	HRESULT CreateConstantBuffer();

	ID3D11SamplerState** GetDefaultSampler()
	{
		return &m_TextureSampler;
	}

public:
	void Init();
	void Update();
	void Render();

};
