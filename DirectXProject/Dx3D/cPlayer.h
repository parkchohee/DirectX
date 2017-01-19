#pragma once

#define GUNMAX 3

#include "cCharacter.h"

class cGun;
class cController;

class cPlayer : public cCharacter
{
	std::vector<cGun*>	m_vecGun;

	int					m_nSelectGun;

	D3DXVECTOR3			m_vCamAngle;

public:
	cPlayer();
	~cPlayer();

	void Setup();
	void Update(D3DXVECTOR3& camAngle, iMap* pMap = NULL);
	void Render();

	cGun* GetGun();
	void GunSetting(D3DXVECTOR3 & camAngle);

private:
	void BulletFire();


};

