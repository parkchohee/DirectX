#pragma once

#include "cCharacter.h"

class cSkinnedMesh;
class cOBB;

class cZealot
{
private:
	cSkinnedMesh*	m_pSkinnedMesh;
	cOBB*			m_pOBB;	/// >> : OBB
	SYNTHESIZE_ADD_REF(cCharacter*, m_pCharacterController, CharacterController);
	
public:
	cZealot(void);
	virtual ~cZealot(void);

	void Setup();
	void Update(iMap* pMap);
	void Render(D3DCOLOR c = D3DCOLOR_XRGB(255, 255, 255));

	/// >> : OBB
	cOBB* GetOBB()
	{
		return m_pOBB;
	}
};

