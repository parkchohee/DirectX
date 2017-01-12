#pragma once

class cGun;
class cPlayer : public cObject
{
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPosition, Position);

	cGun*			m_pGun;

public:
	cPlayer();
	~cPlayer();

	void Setup();
	void Update();
	void Render();

};

