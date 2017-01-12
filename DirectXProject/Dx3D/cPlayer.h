#pragma once

class cSkinnedMesh;

class cPlayer : public cObject
{
	cSkinnedMesh*				m_pShotGun;

	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPosition, Position);

public:
	cPlayer();
	~cPlayer();

	void Setup();
	void Update();
	void Render();

	void GunSetting();
};

