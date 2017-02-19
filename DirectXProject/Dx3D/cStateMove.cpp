#include "stdafx.h"
#include "cStateMove.h"
#include "cBuildingGroup.h"
#include "cBuilding.h"
#include "cCharacter.h"
#include "cOBB.h"


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
	if (!m_pTarget) return;

	m_eStateType = STATE_MOVE;

	m_fPassedDistance = 0.f;
	m_pTarget->SetPosition(m_vFrom);
	
	D3DXVECTOR3 vCenter = m_pBuildings->GetCenter();
	float scope = m_pBuildings->GetBuildingScope();
	vCenter.x -= scope;
	vCenter.z -= scope;
	scope = m_pBuildings->GetScope();

	D3DXVECTOR3 vTo = D3DXVECTOR3(vCenter.x + rand() % ((int)scope * 2), 0, vCenter.z + rand() % ((int)scope * 2));
	D3DXVECTOR3 vDir = vTo - m_vFrom;

	float fDist = D3DXVec3Length(&vDir);
	float fMinDist = fDist;

	D3DXVec3Normalize(&vDir, &vDir);

	for (size_t buildingNum = 0; buildingNum < m_pBuildings->GetBuilding().size(); buildingNum++)
	{
		D3DXVECTOR3 buildingPos = m_pBuildings->GetBuilding()[buildingNum]->GetPosition();
		D3DXVECTOR3 buildingDir = (buildingPos - m_vFrom);

		if (D3DXVec3Length(&buildingDir) > fDist)
			continue;

		D3DXVECTOR3* vertex = m_pBuildings->GetBuilding()[buildingNum]->GetOBB()->GetVertes();
		int index[24] = {0,1,2,0,2,3,3,2,6,3,6,7,7,6,5,7,5,4,4,5,1,0,1,3};
		for (size_t i = 0; i < 24; i += 3)
		{
			float u, v, dist;
			if (D3DXIntersectTri(&vertex[index[i]], &vertex[index[i + 1]], &vertex[index[i + 2]],
				&m_vFrom, &vDir, &u, &v, &dist))
			{
				if (dist < fMinDist)
					fMinDist = dist;
			}
		}
	}

	m_vDir = vDir * fMinDist * 0.7f;
	m_vTo = m_vFrom + m_vDir;
	m_fDistance = D3DXVec3Length(&m_vDir);
	m_pTarget->SetDirection(vDir);
	m_vDir = vDir;
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
	/*m_pTarget->SetPosition(m_vTo);
	m_vFrom = m_vTo;
	m_vTo = D3DXVECTOR3(rand() % 10, 0, rand() % 10);

	Start();
	*/
	m_pTarget->SetPosition(m_vTo);
	m_vFrom = m_vTo;
	Start();
	// 지금 그냥 랜덤한곳으로 이동하도록함..
	// 건물피해서 랜덤한곳으로 이동하도록 해야함.

}
