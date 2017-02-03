#pragma once

#include "cCharacter.h"

class cGun;
class cController;
class cOBB;

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
	void Update(iMap* pMap);
	void Render();

	void SetBoundingSphere();
	ST_SPHERE GetBoundingSphere();
	std::vector<ST_SPHERE> GetBoundingSphereDetail();

private:
	void UpdateSkinnedMesh(D3DXVECTOR3 &vAngle);
};

