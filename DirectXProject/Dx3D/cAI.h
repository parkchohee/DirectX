#pragma once
#include "cCharacter.h"

class cGun;
class cController;

class cAI : public cCharacter
{
	cGun*				m_pGun;

public:
	cAI();
	virtual ~cAI();

	virtual void Setup(char* szFolder, char* szFilename) override;
	virtual void Update(iMap* pMap) override;
	virtual void Render() override;
};

