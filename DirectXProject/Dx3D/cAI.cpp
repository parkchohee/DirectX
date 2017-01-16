#include "stdafx.h"
#include "cAI.h"


cAI::cAI()
	: m_pGun(NULL)
{
}


cAI::~cAI()
{
}

void cAI::Setup(char* szFolder, char* szFilename)
{
	cCharacter::Setup(szFolder, szFilename);
}

void cAI::Update(iMap * pMap)
{
	cCharacter::Update(NULL);
}

void cAI::Render()
{
	cCharacter::Render();
}
