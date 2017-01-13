#pragma once

class cSkinnedMesh;

class cBullet
{
	cSkinnedMesh*			m_pBulletEffect;

public:
	cBullet();
	~cBullet();

	void Setup();
	void Update();
	void Render();

};

