#include "StdAfx.h"
#include "cActionJump.h"


cActionJump::cActionJump(void)
	: m_fGravity(-0.05f)
	, m_fVelY(0.0f)
	//, m_isJump(false)
	, m_pMap(NULL)
	, m_vStartPosition(0, 0, 0)
	, m_vPrevPosition(0, 0, 0)
	, m_fPrevMapY(0.0f)
{
}


cActionJump::~cActionJump(void)
{
}

void cActionJump::Start()
{
	m_vStartPosition = m_pOwner->GetPosition();
	m_vPrevPosition = m_pOwner->GetPosition();
	m_fVelY = 0.8f;
}

void cActionJump::Update()
{
	float fTargetX = (m_pOwner->GetPosition() + m_pOwner->GetDirection() * 0.1f).x;
	float fTargetZ = (m_pOwner->GetPosition() + m_pOwner->GetDirection() * 0.1f).z;
	
	m_fVelY += m_fGravity;
	float fTargetY = m_pOwner->GetPosition().y + m_fVelY;

	if(m_pMap)
	{
		float fMapY = 0.0f;
		if(m_pMap->GetHeight(fTargetX, fMapY, fTargetZ))
		{
			m_fPrevMapY = fMapY;
			if(fTargetY < fMapY)
			{
				m_pOwner->SetPosition(D3DXVECTOR3(fTargetX, fMapY, fTargetZ));
				m_pDelegate->OnActionFinish(this);
			}
			else
			{
				m_pOwner->SetPosition(D3DXVECTOR3(fTargetX, fTargetY, fTargetZ));
			}
		}
		else
		{
			if(fTargetY < m_fPrevMapY)
			{
				m_pOwner->SetPosition(D3DXVECTOR3(m_vPrevPosition.x, m_fPrevMapY, m_vPrevPosition.z));
				m_pDelegate->OnActionFinish(this);
			}
			else
			{
				m_pOwner->SetPosition(D3DXVECTOR3(m_vPrevPosition.x, fTargetY, m_vPrevPosition.z));
			}
		}
	}
	else
	{
// 		if(vTargetPos.y < m_vStartPosition.y)
// 		{
// 			vTargetPos.y = m_vStartPosition.y;
// 			if (m_pDelegate)
// 			{
// 				m_pDelegate->OnActionFinish(this);
// 			}
// 		}
// 		m_pOwner->SetPosition(vTargetPos);
	}

	m_vPrevPosition = m_pOwner->GetPosition();
}
