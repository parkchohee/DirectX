#include "stdafx.h"
#include "cTexQuad.h"


cTexQuad::cTexQuad()
{
	Init();
}


cTexQuad::~cTexQuad()
{
	Release();
}


void cTexQuad::Init()
{

	//Quad 하나 만든다.
	// 1---2
	// |  /|
	// | / |
	// |/  |
	// 0---3

	ST_PT_VERTEX Quad[6];
	Quad[0].p = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	Quad[1].p = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	Quad[2].p = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	Quad[3].p = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	Quad[4].p = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	Quad[5].p = D3DXVECTOR3(1.0f, -1.0f, 0.0f);

	Quad[0].t = D3DXVECTOR2(0, 1);
	Quad[1].t = D3DXVECTOR2(0, 0);
	Quad[2].t = D3DXVECTOR2(1, 0);
	Quad[3].t = D3DXVECTOR2(0, 1);
	Quad[4].t = D3DXVECTOR2(1, 0);
	Quad[5].t = D3DXVECTOR2(1, 1);

	//정점 버퍼 셋팅
	g_pD3DDevice->CreateVertexBuffer(
		sizeof(ST_PT_VERTEX) * 6,
		D3DUSAGE_WRITEONLY,
		ST_PT_VERTEX::FVF,
		D3DPOOL_DEFAULT,
		&this->vb,
		NULL);

	ST_PT_VERTEX* p = NULL;
	this->vb->Lock(0, 0, (void**)&p, 0);
	memcpy(p, Quad, sizeof(ST_PT_VERTEX) * 6);
	this->vb->Unlock();



}
void cTexQuad::Release()
{
	SAFE_RELEASE(this->vb);
}

void cTexQuad::Render()
{
	g_pD3DDevice->SetStreamSource(0, this->vb, 0, sizeof(ST_PT_VERTEX));
	g_pD3DDevice->SetFVF(ST_PT_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
}