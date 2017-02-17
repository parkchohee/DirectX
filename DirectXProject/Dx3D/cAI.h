#pragma once

#include "cCharacter.h"

class cUIImageView;

class cAI : public cCharacter
{
	cSkinnedMesh*			m_pSkinnedMesh;				// Ä³¸¯ÅÍ ¸öÃ¼

	LPD3DXMESH				m_pBoundingSphereDetailMesh;
	std::vector<ST_SPHERE>	m_vecBoundingSphereDetail;

	cOBB*					m_pAIOBB;

	cUIImageView*			m_pAIPointPos;
	LPD3DXSPRITE			m_pSprite;

	SYNTHESIZE(bool, m_isShow, ShowPoint);

public:
	cAI();
	~cAI();

	void Setup(char* szFolder, char* szFilename);
	void Update(D3DXVECTOR3 vPlayer = D3DXVECTOR3(0,0,0), float fAngle = 0.0f);
	void Render();

	void SetHeightMap(cHeightMap* hMap);
	void SetTextMap(cTextMap* tMap);

	ST_SPHERE GetBoundingSphere();
	std::vector<ST_SPHERE> GetBoundingSphereDetail();

	void BulletFire(D3DXVECTOR3 dir);

	void Destroy();
	bool IsDeath();

private:
	void SetBoundingSphere();
	void UpdateSkinnedMesh(D3DXVECTOR3 &vDir);
};

