#pragma once

#include "cAction.h"

class cGameObject
	: public cObject
	, public iActionDelegate
{
protected:
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vDirection, Direction);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPosition, Position);

public:
	cGameObject(void);
	virtual ~cGameObject(void);

	virtual void OnActionFinish(cAction* pSender) override;
};

