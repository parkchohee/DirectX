#include "StdAfx.h"
#include "cCharacter.h"
#include "cSkinnedMesh.h"
#include "cOBB.h"

cCharacter::cCharacter(void)
	: m_pMap(NULL)
	, m_pSkinnedMesh(NULL)
	, m_pController(NULL)
	, m_pOBB(NULL)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cCharacter::~cCharacter(void)
{
	SAFE_DELETE(m_pSkinnedMesh);
}

void cCharacter::Setup(char* szFolder, char* szFilename)
{
	m_pSkinnedMesh = new cSkinnedMesh(szFolder, szFilename);

	D3DXMATRIXA16 matS, matRX, matRY, matT, matSRT;
	D3DXMatrixScaling(&matS, 0.03f, 0.03f, 0.03f);
	D3DXMatrixRotationX(&matRX, - D3DX_PI / 2);
	D3DXMatrixRotationY(&matRY, D3DX_PI);
	D3DXMatrixIdentity(&matT);
	matSRT = matS * matRX * matRY * matT;

	m_pSkinnedMesh->SetTransform(&matSRT);
	

	m_pOBB = new cOBB;
	m_pOBB->Setup(m_pSkinnedMesh);
}

void cCharacter::Update(iMap* pMap)
{
	m_pMap = pMap;

	{
		if (m_pOBB)
			m_pOBB->Update(m_pSkinnedMesh->GetTransFrom());
	}

}

void cCharacter::Render()
{
	if (m_pSkinnedMesh)
		m_pSkinnedMesh->UpdateAndRender();

	if (m_pOBB)
		m_pOBB->OBBBox_Render(D3DCOLOR_XRGB(255, 0, 0));
}

void cCharacter::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_SPACE:
			break;
		}
		break;
	}
}
