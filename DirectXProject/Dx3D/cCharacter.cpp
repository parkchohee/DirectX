#include "StdAfx.h"
#include "cCharacter.h"
#include "cController.h"
#include "cGun.h"
#include "cOBB.h"

cCharacter::cCharacter(void)
	: m_pGun(NULL)
	, m_pController(NULL)
	, m_pBoundingSphereMesh(NULL)
{
	D3DXMatrixIdentity(&m_matWorldTM);
}


cCharacter::~cCharacter(void)
{
}

void cCharacter::Setup(char* szFolder, char* szFilename)
{
}

void cCharacter::Update()
{
	if (m_pGun)
		m_pGun->Update();
}

void cCharacter::Render()
{
	if (m_pGun)
		m_pGun->Render();
}

bool cCharacter::IsAttacked(float power)
{
	return m_fCurrentHp - power < 0 ? true : false;
}
