#pragma once

#include "cState.h"
class cBuildingGroup;

class cStateMove
	: public cState
	, public iStateDelegate
{
	float			m_fSpeed;
	
	float			m_fPassedDistance;
	float			m_fDistance;

	D3DXVECTOR3		m_vDir;

	SYNTHESIZE(cBuildingGroup*, m_pBuildings, Buildings);

	SYNTHESIZE(D3DXVECTOR3, m_vFrom, From);
	SYNTHESIZE(D3DXVECTOR3, m_vTo, To);

public:
	cStateMove();
	virtual ~cStateMove();

	virtual void Start() override;
	virtual void Update() override;

	// iStateDelegate overrride
	virtual void OnStateFinish(cState* pSender) override;
};

