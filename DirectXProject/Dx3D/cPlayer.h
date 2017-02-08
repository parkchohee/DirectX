#pragma once

#define GUNMAX 3

#include "cCharacter.h"

class cPlayer : public cCharacter
{
	std::vector<cGun*>	m_vecGun;
	int					m_nSelectGun;

	D3DXVECTOR3			m_vCamAngle;

	cOBB*               m_pPlayerOBB;


public:
	cPlayer();
	~cPlayer();

	void Setup();
	void Update(D3DXVECTOR3& camAngle);
	void Render();

	void SetHeightMap(cHeightMap* hMap);
	void SetTextMap(cTextMap* tMap);

	cGun* GetGun();
	void GunSetting(D3DXVECTOR3& camAngle);
	void GunSettingZoom(D3DXVECTOR3& camAngle);

	void BulletFire(D3DXVECTOR3 dir);

};

