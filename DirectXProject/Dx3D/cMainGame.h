#pragma once

class cMainGame
{
private:
	LPDIRECT3D9			m_pD3D;
	LPDIRECT3DDEVICE9	m_pD3DDevice;

public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();

	/*virtual void Foo() = 0;*/
};

