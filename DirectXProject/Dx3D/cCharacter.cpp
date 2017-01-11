#include "StdAfx.h"
#include "cCharacter.h"

cCharacter::cCharacter(void)
	: m_fRotY(0.0f)
	, m_pMap(NULL)
	, m_vPosition(0, 0, 0)
	, m_vDirection(0, 0, -1)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cCharacter::~cCharacter(void)
{
}

void cCharacter::Setup()
{

}

void cCharacter::Update(iMap* pMap)
{
	m_pMap = pMap;

	if(GetKeyState('A') & 0x8000)
	{
		m_fRotY -= 0.1f;
	}
	if(GetKeyState('D') & 0x8000)
	{
		m_fRotY += 0.1f;
	}

	RECT rc;
	GetClientRect(g_hWnd, &rc);

	D3DXMATRIXA16 matR, matT;
	D3DXMatrixRotationY(&matR, m_fRotY);
	m_vDirection = D3DXVECTOR3(0, 0, -1);
	
// 	D3DXVec3Normalize(&m_vDirection, &m_vDirection);
// 	D3DXVECTOR3 vUp(0, 1, 0);
// 	D3DXVECTOR3 vRight;
// 	D3DXVec3Cross(&vRight, &vUp, &m_vDirection);
// 	D3DXVec3Normalize(&vRight, &vRight);
// 	D3DXVec3Cross(&vUp, &m_vDirection, &vRight);
// 	D3DXMatrixLookAtLH(&matR, &D3DXVECTOR3(0, 0, 0), &m_vDirection, &vUp);
// 	D3DXMatrixTranspose(&matR, &matR);

 	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matR);

	D3DXVECTOR3 vPosition = m_vPosition;

	if(GetKeyState('W') & 0x8000)
	{
		vPosition = m_vPosition + (m_vDirection * 0.1f);
	}
	if(GetKeyState('S') & 0x8000)
	{
		vPosition = m_vPosition - (m_vDirection * 0.1f);
	}

	m_vPosition = vPosition;
// 	if(pMap)
// 	{
// 		if(pMap->GetHeight(vPosition.x, vPosition.y, vPosition.z))
// 		{
// 			
// 		}
// 	}

	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorld = matR * matT;
}

void cCharacter::Render()
{

}

D3DXVECTOR3& cCharacter::GetPosition()
{
	return m_vPosition;
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
