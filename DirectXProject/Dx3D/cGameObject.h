#pragma once

#include "cAction.h"

class cGameObject
	: public cObject
	, public iActionDelegate
{
protected:
	SYNTHESIZE(D3DXVECTOR3, m_vDirection, Direction);
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);

public:
	cGameObject(void);
	virtual ~cGameObject(void);

	virtual void OnActionFinish(cAction* pSender) override;
};

