#include "StdAfx.h"
#include "cUIObject.h"


cUIObject::cUIObject(void)
	: m_vPosition(0, 0, 0)
	, m_pParent(NULL)
	, m_stSize(0, 0)
	, m_nTag(0)
	, m_isDrawBounding(false)
	, m_isHidden(false)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cUIObject::~cUIObject(void)
{
}

void cUIObject::AddChild( cUIObject* pChild )
{
	pChild->SetParent(this);
	m_vecChild.push_back(pChild);
}

void cUIObject::Update()
{
	D3DXMatrixIdentity(&m_matWorld);
	m_matWorld._41 = m_vPosition.x;
	m_matWorld._42 = m_vPosition.y;
	m_matWorld._43 = m_vPosition.z;

	if (m_pParent)
	{
		m_matWorld._41 += m_pParent->m_matWorld._41;
		m_matWorld._42 += m_pParent->m_matWorld._42;
		m_matWorld._43 += m_pParent->m_matWorld._43;
	}

	for each(auto c in m_vecChild)
	{
		c->Update();
	}
}

void cUIObject::Render(LPD3DXSPRITE pSprite)
{
	if(m_isHidden)
		return;

	if(m_isDrawBounding)
	{
		std::vector<ST_RHWC_VERTEX> vecVertex;
		vecVertex.reserve(5);
		
		ST_RHWC_VERTEX v;
		
		v.c = D3DCOLOR_XRGB(255, 0, 0);
		v.p = D3DXVECTOR4(m_matWorld._41, m_matWorld._42, 0, 1);
		vecVertex.push_back(v);

		v.c = D3DCOLOR_XRGB(255, 0, 0);
		v.p = D3DXVECTOR4(m_matWorld._41 + m_stSize.nWidth, m_matWorld._42, 0, 1);
		vecVertex.push_back(v);

		v.c = D3DCOLOR_XRGB(255, 0, 0);
		v.p = D3DXVECTOR4(m_matWorld._41 + m_stSize.nWidth, m_matWorld._42 + m_stSize.nHeight, 0, 1);
		vecVertex.push_back(v);

		v.c = D3DCOLOR_XRGB(255, 0, 0);
		v.p = D3DXVECTOR4(m_matWorld._41, m_matWorld._42 + m_stSize.nHeight, 0, 1);
		vecVertex.push_back(v);

		v.c = D3DCOLOR_XRGB(255, 0, 0);
		v.p = D3DXVECTOR4(m_matWorld._41, m_matWorld._42, 0, 1);
		vecVertex.push_back(v);

		g_pD3DDevice->SetFVF(ST_RHWC_VERTEX::FVF);
		g_pD3DDevice->DrawPrimitiveUP(
			D3DPT_LINESTRIP, 4, &vecVertex[0], sizeof(ST_RHWC_VERTEX));
	}

	for each(auto c in m_vecChild)
	{
		c->Render(pSprite);
	}
}

void cUIObject::Destroy()
{
	for each(auto c in m_vecChild)
	{
		c->Destroy();
	}
	this->Release();
}

void cUIObject::SetPosition( float x, float y, float z /*= 0*/ )
{
	m_vPosition.x = x;
	m_vPosition.y = y;
	m_vPosition.z = z;
}

cUIObject* cUIObject::FindChildByTag( int nTag )
{
	if(m_nTag == nTag)
		return this;

	for each(auto c in m_vecChild)
	{
		cUIObject* p = c->FindChildByTag(nTag);
		if(p) return p;
	}

	return NULL;
}

void cUIObject::SetHidden( bool isHidden )
{
	for each(auto c in m_vecChild)
	{
		c->SetHidden(isHidden);
	}
	m_isHidden = isHidden;
}
