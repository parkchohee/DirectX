#pragma once

#include "cState.h"
class cAI;

class cStateAttack
	: public cState
	, public iStateDelegate
{
	cAI*		m_pAI;
	D3DXVECTOR3		m_vDir;

	SYNTHESIZE(D3DXVECTOR3, m_vPlayerPos, PlayerPos);
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);

public:
	cStateAttack();
	virtual ~cStateAttack();

	virtual void Start() override;
	virtual void Update() override;

	// iStateDelegate overrride
	virtual void OnStateFinish(cState* pSender) override;
};

