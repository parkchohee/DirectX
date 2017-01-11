#include "StdAfx.h"
#include "cCubeMan.h"
#include "cBody.h"
#include "cLArm.h"
#include "cRArm.h"
#include "cLLeg.h"
#include "cRLeg.h"
#include "cHead.h"

cCubeMan::cCubeMan(void)
	: m_pRoot(NULL)
{
}

cCubeMan::~cCubeMan(void)
{
	SAFE_RELEASE(m_pTexture);

	if(m_pRoot)
		m_pRoot->Destroy();
}

void cCubeMan::Setup()
{
	cCharacter::Setup();

	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	m_stMtl.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	m_stMtl.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

	D3DXCreateTextureFromFile(g_pD3DDevice, "batman.png", &m_pTexture);

	cBody* pBody = new cBody;
	pBody->Setup();
	pBody->SetParentWorldTM(&m_matWorld);

	m_pRoot = pBody;
	

	cHead* pHead = new cHead;
	pHead->Setup();

	m_pRoot->AddChild(pHead);


	cLArm* pLArm = new cLArm;
	pLArm->Setup();
	pLArm->SetRotDeltaX(0.1f);

	m_pRoot->AddChild(pLArm);


	cRArm* pRArm = new cRArm;
	pRArm->Setup();
	pRArm->SetRotDeltaX(-0.1f);

	m_pRoot->AddChild(pRArm);


	cLLeg* pLLeg = new cLLeg;
	pLLeg->Setup();
	pLLeg->SetRotDeltaX(-0.1f);

	m_pRoot->AddChild(pLLeg);


	cRLeg* pRLeg = new cRLeg;
	pRLeg->Setup();
	pRLeg->SetRotDeltaX(0.1f);

	m_pRoot->AddChild(pRLeg);
}

void cCubeMan::Update(iMap* pMap)
{
	cCharacter::Update(pMap);
	if(m_pRoot)
		m_pRoot->Update();
}

void cCubeMan::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetMaterial(&m_stMtl);

	cCharacter::Render();

	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->SetTexture(0, m_pTexture);

	if(m_pRoot)
		m_pRoot->Render();
}
