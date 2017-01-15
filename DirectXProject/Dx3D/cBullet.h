#pragma once

class cBullet : public cGameObject
{
	std::vector<ST_PC_VERTEX>	m_vecVertex;				// 파티클 Vertex

	float						m_fBulletSpeed;				// 총알의 이동 속도
	
	SYNTHESIZE(float, m_fMoveDistance, MoveDistance);		// 총알이 이동한 거리

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

