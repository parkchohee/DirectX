#include "StdAfx.h"
#include "cUIImageView.h"


cUIImageView::cUIImageView(void)
	: m_pTexture(NULL)
	, m_fRateX(1.0f)
	, m_fRateY(1.0f)
{
}


cUIImageView::~cUIImageView(void)
{
}

void cUIImageView::SetTexture( char* szFullPath )
{
	D3DXIMAGE_INFO stImageInfo;
	
	m_pTexture = g_pTextureManager->GetTexture(szFullPath, &stImageInfo);

	m_stSize.nWidth = stImageInfo.Width;
	m_stSize.nHeight = stImageInfo.Height;
}

void cUIImageView::Render( LPD3DXSPRITE pSprite )
{
	if(m_isHidden)
		return;

	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	
	D3DXMATRIXA16 matR, matT;
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixRotationZ(&matR, m_fAngle);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorld = m_matWorld * matR * matT;

	pSprite->SetTransform(&m_matWorld);

	if (m_fRateX < 0.f)
		m_fRateX = 0.f;
	else if (m_fRateX > 1.f)
		m_fRateX = 1.f;

	if (m_fRateY < 0.f)
		m_fRateY = 0.f;
	else if (m_fRateY > 1.f)
		m_fRateY = 1.f;

	RECT rc;
	SetRect(&rc, 0, 0, m_stSize.nWidth * m_fRateX, m_stSize.nHeight * m_fRateY);
	pSprite->Draw(m_pTexture,
		&rc,
		&D3DXVECTOR3(m_stSize.nWidth / 2, m_stSize.nHeight / 2, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_ARGB(255, 255, 255, 255));

	pSprite->End();

	cUIObject::Render(pSprite);
}
