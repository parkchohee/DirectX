#pragma once
class cUITextView : public cUIObject
{
protected:
	SYNTHESIZE(cFontManager::eFontType, m_eFontType, FontType);
	SYNTHESIZE(std::string, m_sText, Text);
	SYNTHESIZE(DWORD, m_dwDrawTextFormat, DrawTextFormat);
	SYNTHESIZE(D3DCOLOR, m_dwTextColor, TextColor);

public:
	cUITextView(void);
	virtual ~cUITextView(void);

	virtual void Render(LPD3DXSPRITE pSprite) override;
};

