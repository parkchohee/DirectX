#include "stdafx.h"
#include "cEvent.h"
#include "cUIImageView.h"
#include "cUIObject.h"


cEvent::cEvent()
	: m_nCount(0)
	, m_pSprite(NULL)
	, m_isPlay(true)
	, m_nTexturNum(0)
	, m_nAlpha(15)
{
}


cEvent::~cEvent()
{
}

void cEvent::Setup(char * FirstszFullPath, char * SecondszFullPath)
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	D3DXIMAGE_INFO stImageInfo;
	m_aTexture[0] = g_pTextureManager->GetTexture(FirstszFullPath, &stImageInfo);
	m_nWidth[0] = stImageInfo.Width;
	m_nHeight[0] = stImageInfo.Height;

	m_aTexture[1] = g_pTextureManager->GetTexture(SecondszFullPath, &stImageInfo);
	m_nWidth[1] = stImageInfo.Width;
	m_nHeight[1] = stImageInfo.Height;
}

void cEvent::Update()
{
	m_nCount++;
	
	if (m_nCount < 120)
	{
		m_nAlpha += 2;
		/*if(m_nCount < 60)
			m_nAlpha += 4;
		else
			m_nAlpha -= 4;
*/
		m_nTexturNum = 0;
	}
	else if (m_nCount > 120 && m_nCount < 240)
	{
		m_nAlpha -= 2;
		/*if (m_nCount < 180)
			m_nAlpha += 4;
		else
			m_nAlpha -= 4;
*/
		m_nTexturNum = 1;
	}
	else if (m_nCount > 240)
	{
		m_isPlay = false;
	}
}

void cEvent::Render()
{
	D3DXMATRIXA16 m_matWorld;
	D3DXMatrixIdentity(&m_matWorld);

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	m_pSprite->SetTransform(&m_matWorld);

	RECT rc;
	SetRect(&rc, 0,0,
		/*WINSIZEX - m_nWidth[m_nTexturNum] / 2,
		WINSIZEY - m_nHeight[m_nTexturNum] / 2,*/
		m_nWidth[m_nTexturNum], m_nHeight[m_nTexturNum]);
	m_pSprite->Draw(m_aTexture[m_nTexturNum],
		&rc,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(CENTERX - m_nWidth[m_nTexturNum] / 2, CENTERY - m_nHeight[m_nTexturNum] / 2 - 150, 0),
		D3DCOLOR_ARGB(m_nAlpha, 255, 255, 255));

	m_pSprite->End();

}

void cEvent::Destroy()
{
	SAFE_RELEASE(m_pSprite);
}
