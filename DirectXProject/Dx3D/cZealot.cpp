#include "StdAfx.h"
#include "cZealot.h"
#include "cSkinnedMesh.h"
#include "cOBB.h"

cZealot::cZealot(void)
	: m_pSkinnedMesh(NULL)
	, m_pCharacterController(NULL)
	, m_pOBB(NULL)	/// : OBB
{
}


cZealot::~cZealot(void)
{
	SAFE_RELEASE(m_pCharacterController);
	SAFE_DELETE(m_pSkinnedMesh);
}

void cZealot::Setup()
{
	m_pSkinnedMesh = new cSkinnedMesh("Zealot/", "zealot.X");
	m_pSkinnedMesh->SetRandomTrackPosition();
	
	/// >> : OBB
	{
		m_pOBB = new cOBB;
		m_pOBB->Setup(m_pSkinnedMesh);
	}
}

void cZealot::Update(iMap* pMap)
{
	if(m_pCharacterController)
		m_pCharacterController->Update(pMap);

	/// >> : OBB
	{
		if (m_pOBB)
			m_pOBB->Update(m_pCharacterController ? m_pCharacterController->GetTransform() : NULL);
	}
}

void cZealot::Render(D3DCOLOR c)
{
	if(m_pCharacterController)
		m_pSkinnedMesh->SetTransform(m_pCharacterController->GetTransform());

	m_pSkinnedMesh->UpdateAndRender();

	/// >> : OBB
	{
		if (m_pOBB)
			m_pOBB->OBBBox_Render(c);
	}
}
