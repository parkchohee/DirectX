#pragma once

class cUIImageView;
class cUIObject;

class cEvent : public cObject
{
	LPD3DXSPRITE		m_pSprite;

	LPDIRECT3DTEXTURE9	m_aTexture[2];
	int					m_nTexturNum;
	int					m_nWidth[2];
	int					m_nHeight[2];

	int                 m_nCount;
	int					m_nAlpha;

	SYNTHESIZE(bool, m_isPlay, IsPlay);

public:
	cEvent();
	~cEvent();

	void Setup(char * FirstszFullPath, char * SecondszFullPath);
	void Update();
	void Render();

	void Destroy();

};

