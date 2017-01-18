#include "stdafx.h"
#include "cAI.h"
#include "cAiController.h"
#include "cGun.h"
#include "cSkinnedMesh.h"

cAI::cAI()
	: m_pGun(NULL)
	, m_pController(NULL)
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
	m_pSkinnedMesh->SetScaling(D3DXVECTOR3(0.01f,0.01f, 0.01f));
	m_pSkinnedMesh->SetRotation(D3DXVECTOR3(-D3DX_PI / 2, D3DX_PI, 0));
	m_pSkinnedMesh->SetPosition(D3DXVECTOR3(0, 0, 0));
	
	m_pController = new cAIController;
	m_pController->Setup(0.1f);
	
	D3DXMATRIXA16* matParent, matRX, matRY, matRZ, matR;
	matParent = m_pSkinnedMesh->getMatrix("LeftHandThumb1");
	m_pGun = new cGun;
	m_pGun->Setup(&m_vPosition, "Gun/", "MG_42.X");
	m_pGun->SetParentWorldMatrix(*matParent);

	D3DXMatrixRotationX(&matRX, 1.378811);
	D3DXMatrixRotationY(&matRY, -0.872665);
	D3DXMatrixRotationZ(&matRZ, 2.827430);

	matR = matRX * matRY * matRZ;

	m_pGun->SetWorldMatrix(matR);
}

void cAI::Update(iMap * pMap)
{
	if (m_pGun)
		m_pGun->Update();
}

void cAI::Render()
{
	if (m_pSkinnedMesh)
		m_pSkinnedMesh->UpdateAndRender();

	if (m_pGun)
		m_pGun->Render();
}
