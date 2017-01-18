#include "stdafx.h"
#include "cAI.h"
#include "cAiController.h"
#include "cGun.h"
#include "cSkinnedMesh.h"
#include "cOBB.h"

cAI::cAI()
	: m_pGun(NULL)
	, m_pController(NULL)
	, m_pOBB(NULL)
{
}


cAI::~cAI()
{
	SAFE_RELEASE(m_pController);
	SAFE_RELEASE(m_pGun);
}

void cAI::Setup(char* szFolder, char* szFilename)
{
	m_pSkinnedMesh = new cSkinnedMesh(szFolder, szFilename);
	
	D3DXMATRIXA16 matS, matRX, matRY, matR, matT, matSRT;
	D3DXMatrixScaling(&matS, 0.03f, 0.03f, 0.03f);
	D3DXMatrixRotationX(&matRX, -D3DX_PI / 2);
	D3DXMatrixRotationY(&matRY, D3DX_PI);
	D3DXMatrixIdentity(&matT);
	D3DXMatrixTranslation(&matT, 1, 0, (rand() % 10));
	matSRT = matS */* matRX * matRY **/matT;
	m_pSkinnedMesh->SetSRT(matSRT);
	

	m_pOBB = new cOBB;
	m_pOBB->Setup(m_pSkinnedMesh);

	//cCharacter::Setup(szFolder, szFilename);

	m_pController = new cAIController;
	m_pController->Setup(0.1f);

}

void cAI::Update(iMap * pMap)
{
	//cCharacter::Update(NULL);

	/*if (m_pController)
		m_pController->Update();*/

	if (m_pOBB)
		m_pOBB->Update(&m_pSkinnedMesh->GetSRT());

}

void cAI::Render()
{
	if (m_pSkinnedMesh)
		m_pSkinnedMesh->UpdateAndRender();

	if (m_pOBB)
		m_pOBB->OBBBox_Render(D3DCOLOR_XRGB(255, 0, 0));
	//cCharacter::Render();
}
