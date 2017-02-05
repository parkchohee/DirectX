#pragma once

#define GUNMAX 3

#include "cCharacter.h"

class cGun;
class cController;
class cOBB;

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
	void Update(D3DXVECTOR3& camAngle, iMap* pHeightMap = NULL, iMap* pTextMap = NULL);
	void Render();

	cGun* GetGun();
	void GunSetting(D3DXVECTOR3& camAngle);
	void GunSettingZoom(D3DXVECTOR3& camAngle);

	void BulletFire();

};

