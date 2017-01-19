#include "stdafx.h"
#include "cAI.h"
#include "cAiController.h"
#include "cGun.h"
#include "cSkinnedMesh.h"

cAI::cAI()
	: m_pBoundingSphereDetailMesh(NULL)
{
}


cAI::~cAI()
{
	SAFE_RELEASE(m_pBoundingSphereDetailMesh);
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
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	
	matR = matRX * matRY * matRZ;
	
	m_matWorldTM = matS * matR * matT;

	m_pSkinnedMesh = new cSkinnedMesh(szFolder, szFilename);
	m_pSkinnedMesh->SetTransform(&m_matWorldTM);

	
	// GUN
	m_pGun = new cGun;
	m_pGun->Setup(&m_vPosition, "Gun/MG_42/", "MG_42.X");
	//m_pGun->SetParentWorldMatrix(m_pSkinnedMesh->getMatrix("RightHandThumb1"));
	m_pGun->SetParentWorldMatrix(m_pSkinnedMesh->getMatrix("RightHandIndex1"));

	D3DXMatrixRotationX(&matRX, 0/*1.378811*/);
	D3DXMatrixRotationY(&matRY, 0/*-0.872665*/);
	D3DXMatrixRotationZ(&matRZ, 0/*2.827430*/);

	matR = matRX * matRY * matRZ;

	m_pGun->SetWorldMatrixByBoneName(&matR, "stock");




	m_pController = new cAIController;
	m_pController->Setup(0.1f);
	
	D3DXCreateSphere(g_pD3DDevice, AI_BOUNDING_SPHERE_DETAIL_SIZE, 20, 20, &m_pBoundingSphereDetailMesh, NULL);
	D3DXCreateSphere(g_pD3DDevice, AI_BOUNDING_SPHERE_SIZE, 20, 20, &m_pBoundingSphereMesh, NULL);
	
	m_stBoundingSphere.vCenter = m_vPosition;
	m_stBoundingSphere.fRadius = AI_BOUNDING_SPHERE_SIZE;
	m_stBoundingSphere.vCenter.y = AI_BOUNDING_SPHERE_SIZE;

	m_vecBoundingSphereDetail.resize(11);
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

	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	matWorld._41 = m_stBoundingSphere.vCenter.x;
	matWorld._42 = m_stBoundingSphere.vCenter.y;
	matWorld._43 = m_stBoundingSphere.vCenter.z;

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pBoundingSphereMesh->DrawSubset(0);

	for each(auto s in m_vecBoundingSphereDetail)
	{
		D3DXMATRIXA16 matWorld;
		D3DXMatrixIdentity(&matWorld);
		matWorld._41 = s.vCenter.x;
		matWorld._42 = s.vCenter.y;
		matWorld._43 = s.vCenter.z;
		
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

		m_pBoundingSphereDetailMesh->DrawSubset(0);
	}
}

void cAI::SetBoundingSphere()
{
	D3DXMATRIXA16* matSphere;
	ST_SPHERE s;
	s.fRadius = AI_BOUNDING_SPHERE_DETAIL_SIZE;

	matSphere = m_pSkinnedMesh->getMatrix("Jaw");
	s.vCenter = D3DXVECTOR3(matSphere->_41, matSphere->_42, matSphere->_43);
	m_vecBoundingSphereDetail[0] = s;

	matSphere = m_pSkinnedMesh->getMatrix("Spine2");
	s.vCenter = D3DXVECTOR3(matSphere->_41, matSphere->_42, matSphere->_43);
	m_vecBoundingSphereDetail[1] = s;

	matSphere = m_pSkinnedMesh->getMatrix("Hips");
	s.vCenter = D3DXVECTOR3(matSphere->_41, matSphere->_42, matSphere->_43);
	m_vecBoundingSphereDetail[2] = s;

	matSphere = m_pSkinnedMesh->getMatrix("LeftUpLeg");
	s.vCenter = D3DXVECTOR3(matSphere->_41, matSphere->_42, matSphere->_43);
	m_vecBoundingSphereDetail[3] = s;

	matSphere = m_pSkinnedMesh->getMatrix("LeftLeg");
	s.vCenter = D3DXVECTOR3(matSphere->_41, matSphere->_42, matSphere->_43);
	m_vecBoundingSphereDetail[4] = s;

	matSphere = m_pSkinnedMesh->getMatrix("LeftFoot");
	s.vCenter = D3DXVECTOR3(matSphere->_41, matSphere->_42, matSphere->_43);
	m_vecBoundingSphereDetail[5] = s;

	matSphere = m_pSkinnedMesh->getMatrix("RightUpLeg");
	s.vCenter = D3DXVECTOR3(matSphere->_41, matSphere->_42, matSphere->_43);
	m_vecBoundingSphereDetail[6] = s;

	matSphere = m_pSkinnedMesh->getMatrix("RightLeg");
	s.vCenter = D3DXVECTOR3(matSphere->_41, matSphere->_42, matSphere->_43);
	m_vecBoundingSphereDetail[7] = s;

	matSphere = m_pSkinnedMesh->getMatrix("RightFoot");
	s.vCenter = D3DXVECTOR3(matSphere->_41, matSphere->_42, matSphere->_43);
	m_vecBoundingSphereDetail[8] = s;

	matSphere = m_pSkinnedMesh->getMatrix("RightForeArm");
	s.vCenter = D3DXVECTOR3(matSphere->_41, matSphere->_42, matSphere->_43);
	m_vecBoundingSphereDetail[9] = s;

	matSphere = m_pSkinnedMesh->getMatrix("LeftForeArm");
	s.vCenter = D3DXVECTOR3(matSphere->_41, matSphere->_42, matSphere->_43);
	m_vecBoundingSphereDetail[10] = s;
}

ST_SPHERE cAI::GetBoundingSphere()
{
	return m_stBoundingSphere;
}

std::vector<ST_SPHERE> cAI::GetBoundingSphereDetail()
{
	return m_vecBoundingSphereDetail;
}
