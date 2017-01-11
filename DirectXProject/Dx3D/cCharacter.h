#pragma once

class iMap;

class cCharacter : public cObject
{
protected:
	float			m_fRotY;
	D3DXVECTOR3		m_vDirection;
	D3DXVECTOR3		m_vPosition;
	D3DXMATRIXA16	m_matWorld;
	iMap*			m_pMap;

public:
	cCharacter(void);
	virtual ~cCharacter(void);

	virtual void Setup();
	virtual void Update(iMap* pMap);
	virtual void Render();
	virtual D3DXVECTOR3& GetPosition();
	virtual D3DXMATRIXA16* GetTransform()
	{
		return &m_matWorld;
	}
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

