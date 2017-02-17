#include "stdafx.h"
#include "cStateMove.h"
#include "cTextMap.h"


cStateMove::cStateMove()
	: m_fSpeed(0.05f)
	, m_fPassedDistance(0.0f)
	, m_fDistance(0.0f)
	, m_vDir(0, 0, 1)
	, m_vFrom(0, 0, 0)
	, m_vTo(0, 0, 0)
{
}


cStateMove::~cStateMove()
{
}

void cStateMove::Start()
{
	if (m_pTarget)
	{
		m_fPassedDistance = 0.f;
		m_pTarget->SetPosition(m_vFrom);
		m_vDir = m_vTo - m_vFrom;
		m_fDistance = D3DXVec3Length(&m_vDir);
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pTarget->SetDirection(m_vDir);
	}
}

void cStateMove::Update()
{
	m_fPassedDistance += m_fSpeed;

	if (m_fDistance < m_fPassedDistance)
	{
		m_pTarget->SetPosition(m_vTo);

		if (m_pDelegate)
			m_pDelegate->OnStateFinish(this);
	}
	else
	{
		D3DXVECTOR3 p = m_vFrom + m_fPassedDistance * m_vDir;
		m_pTarget->SetPosition(p);
	}
}

void cStateMove::OnStateFinish(cState * pSender)
{
	m_pTarget->SetPosition(m_vTo);
	m_vFrom = m_vTo;
	m_vTo = D3DXVECTOR3(rand() % 10, 0, rand() % 10);

	Start();
	

	// 지금 그냥 랜덤한곳으로 이동하도록함..
	// 건물피해서 랜덤한곳으로 이동하도록 해야함.
	// text맵 사용하면 될것같음.

}
