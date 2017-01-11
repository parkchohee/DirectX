#pragma once

class cUIObject : public cObject
{
protected:
	std::vector<cUIObject*> m_vecChild;
	D3DXMATRIXA16			m_matWorld;
	bool					m_isHidden;
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE(cUIObject*, m_pParent, Parent);
	SYNTHESIZE(ST_SIZEN, m_stSize, Size);
	SYNTHESIZE(int, m_nTag, Tag);
	SYNTHESIZE(bool, m_isDrawBounding, DrawBounding);
	//SYNTHESIZE(bool, m_isHidden, Hidden);

public:
	cUIObject(void);
	virtual ~cUIObject(void);

	virtual void AddChild(cUIObject* pChild);
	virtual void Update();
	virtual void Render(LPD3DXSPRITE pSprite);
	virtual void Destroy();
	virtual void SetPosition(float x, float y, float z = 0);
	virtual cUIObject* FindChildByTag(int nTag);
	virtual void SetHidden(bool isHidden);
};

