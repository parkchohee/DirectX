#include "stdafx.h"
#include "cAI.h"
#include "cAiController.h"


cAI::cAI()
	: m_pGun(NULL)
{
}


cAI::~cAI()
{
	SAFE_RELEASE(m_pController);
}

void cAI::Setup(char* szFolder, char* szFilename)
{
	cCharacter::Setup(szFolder, szFilename);

	m_pController = new cAIController;
	m_pController->Setup(0.1f);

}

void cAI::Update(iMap * pMap)
{
	cCharacter::Update(NULL);
}

void cAI::Render()
{
	cCharacter::Render();
}
