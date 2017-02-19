#include "stdafx.h"
#include "cAirDrop.h"
#include "cStaticMesh.h"
#include "cHeightMap.h"


cAirDrop::cAirDrop()
	: m_pAirdrop(NULL)
	, m_pParachute(NULL)
	, m_pAirMan(NULL)
	, m_pHeightMap(NULL)
	, m_fGravity(0.001f)
	, m_isPlay(true)
{
}


cAirDrop::~cAirDrop()
{
}

void cAirDrop::Setup(cHeightMap* heightMap)
{
	m_pHeightMap = heightMap;

	m_vPos = D3DXVECTOR3(0, 30, 0);
	m_vCamPos = D3DXVECTOR3(0, 32, 0);

	m_pAirdrop = g_pStaticMeshManager->GetStaticMesh("Airdrop/", "Airdrop.X");
	m_pParachute = g_pStaticMeshManager->GetStaticMesh("Airdrop/", "Parachute.X");
	m_pAirMan = g_pStaticMeshManager->GetStaticMesh("Airdrop/", "AirMan.X");
	
	D3DXMATRIXA16 matS, matR, matT, matSRT;

	D3DXMatrixScaling(&matS, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationX(&matR, -D3DX_PI / 2);
	
	matSRT = matS * matR;

	m_pAirdrop->SetWorld(matSRT);
	m_pParachute->SetWorld(matSRT);
	m_pAirMan->SetWorld(matSRT);

	m_pAirdrop->SetPosition(m_vPos);
	m_pParachute->SetPosition(m_vPos);
	m_pAirMan->SetPosition(m_vPos);

}

void cAirDrop::Update()
{
	m_vPos.y -= (0.03f + m_fGravity);
	m_vCamPos.y -= (0.03f + m_fGravity);

	m_fGravity += 0.001f;

	if (m_pHeightMap)
	{
		float height = 0.f;
		m_pHeightMap->GetHeight(m_vPos.x, height, m_vPos.z);
		
		if (m_vPos.y < height)
		{
			m_vPos.y = height;
			m_isPlay = false;
		//	m_vCamPos.y = height + 2;
		}
	}

	m_pAirdrop->SetPosition(m_vPos);
	m_pParachute->SetPosition(m_vPos);
	m_pAirMan->SetPosition(m_vPos);
}

void cAirDrop::Render()
{
	if (m_pAirdrop)
		m_pAirdrop->Render();

	if (m_pParachute)
		m_pParachute->Render();

	if (m_pAirMan)
		m_pAirMan->Render();
}
