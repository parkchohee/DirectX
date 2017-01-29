#include "stdafx.h"
#include "cBuilding.h"
#include "cStaticMesh.h"
#include "cOBB.h"


cBuilding::cBuilding(char* szFolder, char* szFilename)
	: m_vPosition(0,0,0)
	, m_fAngle(0.0f)
	, m_fScale(0.007f)
	, m_pOBB(NULL)	/// : OBB
{
	Setup(szFolder, szFilename);
}


cBuilding::~cBuilding()
{
	SAFE_DELETE(m_pBuilding);
}

void cBuilding::Setup(char* szFolder, char* szFilename)
{
	m_pBuilding = new cStaticMesh(szFolder, szFilename);
	
	m_pOBB = new cOBB;
	m_pOBB->Setup(m_pBuilding);

	m_sFileName = std::string(szFolder) + std::string(szFilename);
}

void cBuilding::Update()
{
	D3DXMATRIXA16 matS, matR, matT, matSRT;
	D3DXMatrixScaling(&matS, m_fScale, m_fScale, m_fScale);
	D3DXMatrixRotationY(&matR, m_fAngle);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	matSRT = matS * matR * matT;
	m_pBuilding->SetWorld(matSRT);

	if (m_pOBB)
		m_pOBB->Update(&matSRT);
}

void cBuilding::Render()
{
	if (m_pBuilding)
		m_pBuilding->Render();

	if (m_pOBB)
		m_pOBB->OBBBox_Render(D3DCOLOR_XRGB(255, 255, 255));
}
