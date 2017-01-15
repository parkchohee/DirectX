#pragma once

class cBullet : public cGameObject
{
	std::vector<ST_PC_VERTEX>	m_vecVertex;				// ��ƼŬ Vertex

	float						m_fBulletSpeed;				// �Ѿ��� �̵� �ӵ�
	
	SYNTHESIZE(float, m_fMoveDistance, MoveDistance);		// �Ѿ��� �̵��� �Ÿ�

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

