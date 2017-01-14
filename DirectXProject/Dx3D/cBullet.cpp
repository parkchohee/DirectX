#include "stdafx.h"
#include "cBullet.h"


DWORD FtoDw(float f)
{
	return *((DWORD*)&f);
}

cBullet::cBullet()
	: m_vDirection(0, 0, 0)
	, m_vPosition(0, 0, 0)
{
}


cBullet::~cBullet()
{
}

void cBullet::Setup(D3DXVECTOR3 & vDirection, D3DXVECTOR3 & vPosition)
{
	m_vDirection = vDirection;
	m_vPosition = vPosition;

	Setup_Particle();

}

void cBullet::Update()
{
	Update_Particle(); 	/// >> : Particle Update

}

void cBullet::Render()
{
	Render_Particle(); /// : particle

}

void cBullet::Setup_Particle()
{
	//m_vecParticle.resize(1000);

	/// >> : 파티클 랜덤하게 생성
	m_vecVertex.resize(1000);
	for (int i = 0; i < m_vecVertex.size(); ++i)
	{
		float fRadius = 0.01;// rand() % 10 / 10.0f;

		m_vecVertex[i].p = D3DXVECTOR3(0, 0, fRadius);

		D3DXVECTOR3 vAngle = D3DXVECTOR3(
			D3DXToRadian(rand() % 3600 / 10.0f),
			D3DXToRadian(rand() % 3600 / 10.0f),
			D3DXToRadian(rand() % 3600 / 10.0f));
		
		D3DXMATRIX matT, matRX, matRY, matRZ, matWorld;
		D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
		D3DXMatrixRotationX(&matRX, vAngle.x);
		D3DXMatrixRotationY(&matRY, vAngle.y);
		D3DXMatrixRotationZ(&matRZ, vAngle.z);
		matWorld = matRX * matRY * matRZ * matT;

		D3DXVec3TransformCoord(
			&m_vecVertex[i].p,
			&m_vecVertex[i].p,
			&matWorld);

		m_vecVertex[i].c = D3DCOLOR_ARGB(255, 180, 70, 20);
	}


	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, false);	/// >> : 포인트 확대 여부 

	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE, FtoDw(5.f));	/// >> : 포인트 사이즈 설정 

																/// : 기본 도형에 대한 거리 사이즈의 감소를 제어 , 포인트 스케일링 Factor값 설정
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));	/// : vs 10.0f check 

	g_pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true); /// : 포인트에 텍스쳐를 입힐 수 있게 해줌
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));	/// : 포인트 최소 크기
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDw(10.0f));	/// : 포인트 최대 크기 vs 10.0f check 

																		// 텍스쳐 알파 옵션 설정
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// 알파블랜딩 방식 결정.
	g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

void cBullet::Update_Particle()
{
	static int nAlpha = 0;
	static int nDelta = 4;
	nAlpha += nDelta;
	if (nAlpha > 255)
	{
		nAlpha = 255;
		nDelta *= -1;
	}
	if (nAlpha < 0)
	{
		nAlpha = 0;
		nDelta *= -1;
	}
	for (int i = 0; i < m_vecVertex.size(); ++i)
	{
	/*	if (i % 2)
			continue;*/
		m_vecVertex[i].p = m_vecVertex[i].p + D3DXVECTOR3(0.1 * m_vDirection.x, 0.1 * m_vDirection.y, 0.1 * m_vDirection.z);
		m_vecVertex[i].c = D3DCOLOR_ARGB(nAlpha, 180, 70, 20); /// : 알파값만 변경
	}
}

void cBullet::Render_Particle()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->SetTexture(0, g_pTextureManager->GetTexture("alpha_tex.tga"));
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST,
		m_vecVertex.size(),
		&m_vecVertex[0],
		sizeof(ST_PC_VERTEX));

	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}
