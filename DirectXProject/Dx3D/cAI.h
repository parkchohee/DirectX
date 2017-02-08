#pragma once

#include "cCharacter.h"

class cAI : public cCharacter
{
	cSkinnedMesh*			m_pSkinnedMesh;				// Ä³¸¯ÅÍ ¸öÃ¼

	LPD3DXMESH				m_pBoundingSphereDetailMesh;
	std::vector<ST_SPHERE>	m_vecBoundingSphereDetail;

	cOBB*					m_pAIOBB;

public:
	cAI();
	~cAI();

	void Setup(char* szFolder, char* szFilename);
	void Update();
	void Render();

	void SetHeightMap(cHeightMap* hMap);
	void SetTextMap(cTextMap* tMap);

	ST_SPHERE GetBoundingSphere();
	std::vector<ST_SPHERE> GetBoundingSphereDetail();

	void Destroy();
	bool IsDeath();

private:
	void SetBoundingSphere();
	void UpdateSkinnedMesh(D3DXVECTOR3 &vAngle);
};

