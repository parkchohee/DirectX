#include "stdafx.h"
#include "cAI.h"
#include "cAiController.h"
#include "cGun.h"
#include "cSkinnedMesh.h"

cAI::cAI()
	: m_pGun(NULL)
	, m_pController(NULL)
	, m_pBoundingSphereMesh(NULL)
{
}


cAI::~cAI()
{
	SAFE_RELEASE(m_pBoundingSphereMesh);
	SAFE_RELEASE(m_pController);
	SAFE_RELEASE(m_pGun);
}

void cAI::Setup(char* szFolder, char* szFilename)
{
	// SkinnedMesh
	D3DXMATRIXA16 matS, matRX, matRY, matRZ, matR, matT;
	D3DXMatrixScaling(&matS, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationX(&matRX, -D3DX_PI / 2);
	D3DXMatrixRotationY(&matRY, D3DX_PI);
	D3DXMatrixRotationZ(&matRZ, 0);
	D3DXMatrixTranslation(&matT, 0, 0, 0);
	
	matR = matRX * matRY * matRZ;
	
	m_matWorldTM = matS * matR * matT;

	m_pSkinnedMesh = new cSkinnedMesh(szFolder, szFilename);
	m_pSkinnedMesh->SetTransform(&m_matWorldTM);

	// GUN
	m_pGun = new cGun;
	m_pGun->Setup(&m_vPosition, "Gun/MG_42/", "MG_42.X");
	m_pGun->SetParentWorldMatrix(m_pSkinnedMesh->getMatrix("RightHandThumb1"));

	D3DXMatrixRotationX(&matRX, 1.378811);
	D3DXMatrixRotationY(&matRY, -0.872665);
	D3DXMatrixRotationZ(&matRZ, 2.827430);

	matR = matRX * matRY * matRZ;

	m_pGun->SetWorldMatrix(&matR, "stock");

	m_pController = new cAIController;
	m_pController->Setup(0.1f);
	
	D3DXCreateSphere(g_pD3DDevice, 0.2f, 20, 20, &m_pBoundingSphereMesh, NULL);
	m_vecSphere.resize(11);
}

void cAI::Update(iMap * pMap)
{
	if (m_pGun)
		m_pGun->Update();

	SetBoundingSphere();
}

void cAI::Render()
{
	if (m_pSkinnedMesh)
		m_pSkinnedMesh->UpdateAndRender();

	if (m_pGun)
		m_pGun->Render();

	for each(auto s in m_vecSphere)
	{
		D3DXMATRIXA16 matWorld;
		D3DXMatrixIdentity(&matWorld);
		matWorld._41 = s.vCenter.x;
		matWorld._42 = s.vCenter.y;
		matWorld._43 = s.vCenter.z;
		
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

		m_pBoundingSphereMesh->DrawSubset(0);
	}
}

void cAI::SetBoundingSphere()
{
	D3DXMATRIXA16* matSphere;
	ST_SPHERE s;

	matSphere = m_pSkinnedMesh->getMatrix("Jaw");
	s.vCenter = D3DXVECTOR3(matSphere->_41, matSphere->_42, matSphere->_43);
	m_vecSphere[0] = s;

	matSphere = m_pSkinnedMesh->getMatrix("Spine2");
	s.vCenter = D3DXVECTOR3(matSphere->_41, matSphere->_42, matSphere->_43);
	m_vecSphere[1] = s;

	matSphere = m_pSkinnedMesh->getMatrix("Hips");
	s.vCenter = D3DXVECTOR3(matSphere->_41, matSphere->_42, matSphere->_43);
	m_vecSphere[2] = s;

	matSphere = m_pSkinnedMesh->getMatrix("LeftUpLeg");
	s.vCenter = D3DXVECTOR3(matSphere->_41, matSphere->_42, matSphere->_43);
	m_vecSphere[3] = s;

	matSphere = m_pSkinnedMesh->getMatrix("LeftLeg");
	s.vCenter = D3DXVECTOR3(matSphere->_41, matSphere->_42, matSphere->_43);
	m_vecSphere[4] = s;

	matSphere = m_pSkinnedMesh->getMatrix("LeftFoot");
	s.vCenter = D3DXVECTOR3(matSphere->_41, matSphere->_42, matSphere->_43);
	m_vecSphere[5] = s;

	matSphere = m_pSkinnedMesh->getMatrix("RightUpLeg");
	s.vCenter = D3DXVECTOR3(matSphere->_41, matSphere->_42, matSphere->_43);
	m_vecSphere[6] = s;

	matSphere = m_pSkinnedMesh->getMatrix("RightLeg");
	s.vCenter = D3DXVECTOR3(matSphere->_41, matSphere->_42, matSphere->_43);
	m_vecSphere[7] = s;

	matSphere = m_pSkinnedMesh->getMatrix("RightFoot");
	s.vCenter = D3DXVECTOR3(matSphere->_41, matSphere->_42, matSphere->_43);
	m_vecSphere[8] = s;

	matSphere = m_pSkinnedMesh->getMatrix("RightForeArm");
	s.vCenter = D3DXVECTOR3(matSphere->_41, matSphere->_42, matSphere->_43);
	m_vecSphere[9] = s;

	matSphere = m_pSkinnedMesh->getMatrix("LeftForeArm");
	s.vCenter = D3DXVECTOR3(matSphere->_41, matSphere->_42, matSphere->_43);
	m_vecSphere[10] = s;
}
