#pragma once

#include "cAction.h"

class cActionBezier : public cAction
{
protected:
	DWORD m_fStartTime;
	float m_fPassedTime;

	D3DXVECTOR3 m_vPrevPosition;
	SYNTHESIZE(float, m_fActionTime, ActionTime);
	SYNTHESIZE(D3DXVECTOR3, m_vFrom, From);
	SYNTHESIZE(D3DXVECTOR3, m_vVia, Via);
	SYNTHESIZE(D3DXVECTOR3, m_vTo, To);
	
public:
	cActionBezier(void);
	virtual ~cActionBezier(void);

	// cAction override
	virtual void Start() override;
	virtual void Update() override;
};

