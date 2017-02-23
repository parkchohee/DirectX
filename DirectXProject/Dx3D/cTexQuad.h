#pragma once
class cTexQuad
{
	LPDIRECT3DVERTEXBUFFER9			vb;

public:
	cTexQuad();
	~cTexQuad();

	void Init();
	void Release();
	void Render();
};

