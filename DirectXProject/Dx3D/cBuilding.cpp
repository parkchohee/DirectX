#include "stdafx.h"
#include "cBuilding.h"
#include "cStaticMesh.h"
#include "cOBB.h"
#include "cObjLoader.h"
#include "cGroup.h"


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
	std::string xFileName(szFilename);
	xFileName += ".X";
	m_pBuilding = new cStaticMesh(szFolder, (char*)xFileName.c_str());
	
	/*m_pOBB = new cOBB;
	m_pOBB->Setup(m_pBuilding);*/

	m_sFolderName = std::string(szFolder);
	m_sFileName = std::string(szFilename);
}

void cBuilding::Update()
{
	D3DXMATRIXA16 matS, matR, matT, matSRT;
	D3DXMatrixScaling(&matS, m_fScale, m_fScale, m_fScale);
	D3DXMatrixRotationY(&matR, m_fAngle);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	matSRT = matS * matR * matT;
	m_pBuilding->SetWorld(matSRT);

	matSRT = matR * matT;

	if (m_pOBB)
		m_pOBB->Update(&matSRT);
}

void cBuilding::Render()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	if (m_pBuilding)
		m_pBuilding->Render();

	if (m_pOBB)
		m_pOBB->OBBBox_Render(D3DCOLOR_XRGB(255, 0, 255));
}

void cBuilding::Init()
{
	m_vPosition = D3DXVECTOR3(0, 0, 0);
	m_fAngle = 0.f;
	m_fScale = 0.007f;
}

void cBuilding::SetOBB(char * szFolder, char * szFilename)
{
	cObjLoader* objLoader = new cObjLoader;
	std::vector<cGroup*>	vecGroup;
	objLoader->Load(vecGroup, szFolder, szFilename);
	if (vecGroup.size() <= 0)
		//°æ°íÃ¢;
		;
	m_pOBB = new cOBB;
	m_pOBB->Setup(vecGroup[0]->GetMin(), vecGroup[0]->GetMax());
}

void cBuilding::SetScale(float scale)
{
	m_fScale = scale; 
	if(m_pOBB)
		m_pOBB->SetScale(scale);
}
