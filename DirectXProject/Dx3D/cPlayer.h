#pragma once

class cGun;
class cPlayer : public cObject
{

	cGun*				m_pGun;
	D3DXVECTOR3			m_vCamAngle;

	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPosition, Position);

public:
	cPlayer();
	~cPlayer();

	void Setup();
	void Update(D3DXVECTOR3& camAngle);
	void Render();

};

