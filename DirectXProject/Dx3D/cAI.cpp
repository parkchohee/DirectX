#include "stdafx.h"
#include "cAI.h"
#include "cAiController.h"
#include "cGun.h"
#include "cOBB.h"
#include "cSkinnedMesh.h"
#include "cTextMap.h"

cAI::cAI()
	: m_pBoundingSphereDetailMesh(NULL)
	, m_pAIOBB(NULL)
{
}


cAI::~cAI()
{
	SAFE_DELETE(m_pAIOBB);
	SAFE_RELEASE(m_pBoundingSphereDetailMesh);
	SAFE_RELEASE(m_pBoundingSphereMesh);
	SAFE_RELEASE(m_pController);
	SAFE_RELEASE(m_pGun);
}

void cAI::Setup(char* szFolder, char* szFilename)
{
	// SkinnedMesh
	m_pSkinnedMesh = new cSkinnedMesh(szFolder, szFilename);

	// obb
	m_pAIOBB = new cOBB;
	m_pAIOBB->Setup(m_pSkinnedMesh, 0.01f);

	// GUN
	m_pGun = new cGun;
	m_pGun->Setup(&m_vPosition, "Gun/MG_42/", "MG_42.X");
	m_pGun->SetParentWorldMatrix(m_pSkinnedMesh->getMatrix("RightHandIndex1"));

	D3DXMATRIXA16 matRX, matRY, matR;
	D3DXMatrixRotationX(&matRX, D3DX_PI / 2);
	D3DXMatrixRotationY(&matRY, -D3DX_PI / 2 - 0.3f);

	matR = matRX * matRY;
	m_pGun->SetWorldMatrixByBoneName(&matR, "stock");

	// Controller
	m_pController = new cAIController;
	m_pController->Setup(0.1f);
	m_pController->SetOBB(m_pAIOBB);
	
	// bounding Sphere
	D3DXCreateSphere(g_pD3DDevice, AI_BOUNDING_SPHERE_DETAIL_SIZE, 20, 20, &m_pBoundingSphereDetailMesh, NULL);
	D3DXCreateSphere(g_pD3DDevice, AI_BOUNDING_SPHERE_SIZE, 20, 20, &m_pBoundingSphereMesh, NULL);
	
	m_stBoundingSphere.fRadius = AI_BOUNDING_SPHERE_SIZE;

	m_vecBoundingSphereDetail.resize(11);
}

void cAI::Update()
{
	if (m_pGun)
		m_pGun->Update();

	D3DXVECTOR3 vAngle = D3DXVECTOR3(0,0,0);
	// angle을 컨트롤러에서 받아와서
	if (m_pController)
		m_pController->Update(vAngle, m_vDirection, m_vPosition);

	UpdateSkinnedMesh(vAngle);
	SetBoundingSphere();

	//if (m_pAIOBB)
	//	m_pAIOBB->Update(&m_matWorldTM);
}

void cAI::Render()
{
	if (m_pAIOBB)
		m_pAIOBB->OBBBox_Render(D3DCOLOR_XRGB(0, 0, 255));

	if (m_pSkinnedMesh)
		m_pSkinnedMesh->UpdateAndRender();

	if (m_pGun)
		m_pGun->Render();

	//D3DXMATRIXA16 matWorld;
	//D3DXMatrixIdentity(&matWorld);
	//matWorld._41 = m_stBoundingSphere.vCenter.x;
	//matWorld._42 = m_stBoundingSphere.vCenter.y;
	//matWorld._43 = m_stBoundingSphere.vCenter.z;

	//g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	//m_pBoundingSphereMesh->DrawSubset(0);
	//
	//for each(auto s in m_vecBoundingSphereDetail)
	//{
	//	D3DXMATRIXA16 matWorld;
	//	D3DXMatrixIdentity(&matWorld);
	//	matWorld._41 = s.vCenter.x;
	//	matWorld._42 = s.vCenter.y;
	//	matWorld._43 = s.vCenter.z;
	//	
	//	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	//	m_pBoundingSphereDetailMesh->DrawSubset(0);
	//}
	//

}

void cAI::SetHeightMap(cHeightMap * hMap)
{
	m_pController->SetHeightMap(hMap);
}

void cAI::SetTextMap(cTextMap * tMap)
{
	m_pController->SetTextMap(tMap);
}

void cAI::SetBoundingSphere()
{
	D3DXMATRIXA16 matSphere, matT;
	ST_SPHERE s;
	s.fRadius = AI_BOUNDING_SPHERE_DETAIL_SIZE;
	D3DXMatrixTranslation(&matT, 0, AI_BOUNDING_SPHERE_SIZE, 0);
	
	s.vCenter = D3DXVECTOR3(0, 0, 0);
	matSphere = *m_pSkinnedMesh->getLocalMatrix("Jaw") * m_matWorldTM;
	D3DXVec3TransformCoord(&s.vCenter, &s.vCenter, &matSphere);
	m_vecBoundingSphereDetail[0] = s;
	
	s.vCenter = D3DXVECTOR3(0, 0, 0);
	matSphere = *m_pSkinnedMesh->getLocalMatrix("Spine2") * m_matWorldTM;
	D3DXVec3TransformCoord(&s.vCenter, &s.vCenter, &matSphere);
	m_vecBoundingSphereDetail[1] = s;
	
	s.vCenter = D3DXVECTOR3(0, 0, 0);
	matSphere = *m_pSkinnedMesh->getLocalMatrix("Hips") * m_matWorldTM;
	D3DXVec3TransformCoord(&s.vCenter, &s.vCenter, &matSphere);
	m_vecBoundingSphereDetail[2] = s;

	s.vCenter = D3DXVECTOR3(0, 0, 0);
	matSphere = *m_pSkinnedMesh->getLocalMatrix("LeftUpLeg") * m_matWorldTM;
	D3DXVec3TransformCoord(&s.vCenter, &s.vCenter, &matSphere);
	m_vecBoundingSphereDetail[3] = s;

	s.vCenter = D3DXVECTOR3(0, 0, 0);
	matSphere = *m_pSkinnedMesh->getLocalMatrix("LeftLeg") * m_matWorldTM;
	D3DXVec3TransformCoord(&s.vCenter, &s.vCenter, &matSphere);
	m_vecBoundingSphereDetail[4] = s;

	s.vCenter = D3DXVECTOR3(0, 0, 0);
	matSphere = *m_pSkinnedMesh->getLocalMatrix("LeftFoot") * m_matWorldTM;
	D3DXVec3TransformCoord(&s.vCenter, &s.vCenter, &matSphere);
	m_vecBoundingSphereDetail[5] = s;

	s.vCenter = D3DXVECTOR3(0, 0, 0);
	matSphere = *m_pSkinnedMesh->getLocalMatrix("RightUpLeg") * m_matWorldTM;
	D3DXVec3TransformCoord(&s.vCenter, &s.vCenter, &matSphere);
	m_vecBoundingSphereDetail[6] = s;

	s.vCenter = D3DXVECTOR3(0, 0, 0);
	matSphere = *m_pSkinnedMesh->getLocalMatrix("RightLeg") * m_matWorldTM;
	D3DXVec3TransformCoord(&s.vCenter, &s.vCenter, &matSphere);
	m_vecBoundingSphereDetail[7] = s;

	s.vCenter = D3DXVECTOR3(0, 0, 0);
	matSphere = *m_pSkinnedMesh->getLocalMatrix("RightFoot") * m_matWorldTM;
	D3DXVec3TransformCoord(&s.vCenter, &s.vCenter, &matSphere);
	m_vecBoundingSphereDetail[8] = s;

	s.vCenter = D3DXVECTOR3(0, 0, 0);
	matSphere = *m_pSkinnedMesh->getLocalMatrix("RightForeArm") * m_matWorldTM;
	D3DXVec3TransformCoord(&s.vCenter, &s.vCenter, &matSphere);
	m_vecBoundingSphereDetail[9] = s;

	s.vCenter = D3DXVECTOR3(0, 0, 0);
	matSphere = *m_pSkinnedMesh->getLocalMatrix("LeftForeArm") * m_matWorldTM;
	D3DXVec3TransformCoord(&s.vCenter, &s.vCenter, &matSphere);
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

void cAI::Destroy()
{
	m_pSkinnedMesh->PlayOneShotAfterHold(8);
}

bool cAI::IsDeath()
{
	return !m_pSkinnedMesh->IsPlay();
}

void cAI::UpdateSkinnedMesh(D3DXVECTOR3 &vAngle)
{
	D3DXMATRIXA16 matS, matRX, matRY, matR, matT;
	D3DXMatrixScaling(&matS, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationX(&matRX, -D3DX_PI / 2 + vAngle.x);
	D3DXMatrixRotationY(&matRY, D3DX_PI + vAngle.y);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	matR = matRX * matRY;

	m_matWorldTM = matS * matR * matT;
	m_pSkinnedMesh->SetTransform(&m_matWorldTM);

	D3DXMATRIXA16 obbWorld;
	obbWorld = matR * matT;
	if (m_pAIOBB)
		m_pAIOBB->Update(&obbWorld);

	m_stBoundingSphere.vCenter = m_vPosition;
	m_stBoundingSphere.vCenter.y += AI_BOUNDING_SPHERE_SIZE;
}
