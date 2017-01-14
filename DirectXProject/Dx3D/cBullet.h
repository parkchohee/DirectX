#pragma once

class cBullet
{
	std::vector<ST_PC_VERTEX>	m_vecVertex;
	D3DXVECTOR3					m_vDirection;
	D3DXVECTOR3					m_vPosition;

public:
	cBullet();
	~cBullet();

	void Setup(D3DXVECTOR3 & vDirection, D3DXVECTOR3 & vPosition);
	void Update();
	void Render();

	/// >> : particle
	void Setup_Particle();
	void Update_Particle();
	void Render_Particle();
	/// << : 
};

