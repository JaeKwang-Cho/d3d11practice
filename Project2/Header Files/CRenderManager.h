#pragma once 

struct MeshComp;

class CRenderManager
{
	SINGLETON(CRenderManager)
private:
	priority_queue<MeshComp*> AlphaAssets;
	vector<vector<MeshComp*>> NonAlphaAssets;

public:
	void Init();
	void Update();
	void Render();

};

