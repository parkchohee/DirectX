#pragma once

#include "cAction.h"

class iMap;

class cActionJump : public cAction
{
protected:
	D3DXVECTOR3	m_vStartPosition;
	D3DXVECTOR3	m_vPrevPosition;
	float		m_fGravity;
	float		m_fVelY;
	float		m_fPrevMapY;
	//bool		m_isJump;
	
	SYNTHESIZE(iMap*, m_pMap, Map);

public:
	cActionJump(void);
	virtual ~cActionJump(void);

	virtual void Start() override;
	virtual void Update() override;
};

