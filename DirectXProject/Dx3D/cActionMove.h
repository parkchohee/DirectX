#pragma once

#include "cAction.h"

class cActionMove : public cAction
{
protected:
	float m_fStartTime;

	float m_fPassedTime;
	
	D3DXVECTOR3 m_vPrevPosition;

	SYNTHESIZE(D3DXVECTOR3, m_vFrom, From);
	SYNTHESIZE(D3DXVECTOR3, m_vTo, To);
	SYNTHESIZE(float, m_fActionTime, ActionTime);

public:
	cActionMove(void);
	virtual ~cActionMove(void);

	virtual void Start() override;
	virtual void Update() override;
};

