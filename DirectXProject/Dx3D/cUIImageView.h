#pragma once

class cUIImageView : public cUIObject
{
	/*float rateX;
	float rateY;*/
	SYNTHESIZE(float, m_fRateX, RateX);
	SYNTHESIZE(float, m_fRateY, RateY);

protected:
	LPDIRECT3DTEXTURE9	m_pTexture;

public:
	cUIImageView(void);
	virtual ~cUIImageView(void);

	virtual void SetTexture(char* szFullPath);

	// cUIObject override
	virtual void Render(LPD3DXSPRITE pSprite) override;
};

