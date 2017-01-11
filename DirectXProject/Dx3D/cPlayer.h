#pragma once

class cSkinnedMesh;

class cPlayer : public cObject
{

	cSkinnedMesh*				m_pShotGun;

public:
	cPlayer();
	~cPlayer();

	void Setup();
	void Update();
	void Render();
};

