#pragma once

class cUIImageView : public cUIObject
{
protected:
	LPDIRECT3DTEXTURE9	m_pTexture;

public:
	cUIImageView(void);
	virtual ~cUIImageView(void);

	virtual void SetTexture(char* szFullPath);

	// cUIObject override
	virtual void Render(LPD3DXSPRITE pSprite) override;
};

