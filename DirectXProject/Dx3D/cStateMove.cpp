#include "stdafx.h"
#include "cStateMove.h"
#include "cBuildingGroup.h"
#include "cBuilding.h"
#include "cCharacter.h"
#include "cOBB.h"
#include "cAI.h"


cStateMove::cStateMove()
	: m_fSpeed(0.05f)
	, m_fPassedDistance(0.0f)
	, m_fDistance(0.0f)
	, m_vDir(0, 0, 1)
	, m_vFrom(0, 0, 0)
	, m_vTo(0, 0, 0)
	, m_isRandomPos(false)
	, m_isSetTo(false)
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

	if (m_isSetTo)
	{
	//	DWORD dwThID;
	//	CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThSetPos, this, NULL, &dwThID));

		SetPos(m_vTo);
		m_isSetTo = false;
		//// to를 randompos에 넣어주면 될것같음
		//m_fDistance = D3DXVec3Length(&(m_vTo - m_vFrom));
		//m_vDir = m_vTo - m_vFrom;
		//D3DXVec3Normalize(&m_vDir, &m_vDir);
		//m_pTarget->SetDirection(m_vDir);
	}
	else
	{
		D3DXVECTOR3 vCenter = m_pBuildings->GetCenter();
		float scope = m_pBuildings->GetBuildingScope();
		scope = m_pBuildings->GetScope();
		vCenter.x -= scope;
		vCenter.z -= scope;
		

		m_vTo = D3DXVECTOR3(vCenter.x + rand() % ((int)scope * 2), 0, vCenter.z + rand() % ((int)scope * 2));

	//	DWORD dwThID;
	//	CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThSetPos, this, NULL, &dwThID));

		SetPos(m_vTo);
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

	Start();
//	SetRandomPos();
}
//
//void cStateMove::SetRandomPos()
//{
//	m_isRandomPos = true;
//	m_fPassedDistance = 0.f;
//	m_pTarget->SetPosition(m_vFrom);
//
//	D3DXVECTOR3 vCenter = m_pBuildings->GetCenter();
//	float scope = m_pBuildings->GetBuildingScope();
//	vCenter.x -= scope;
//	vCenter.z -= scope;
//	scope = m_pBuildings->GetScope();
//
//	D3DXVECTOR3 vTo = D3DXVECTOR3(vCenter.x + rand() % ((int)scope * 2), 0, vCenter.z + rand() % ((int)scope * 2));
//	D3DXVECTOR3 vDir = vTo - m_vFrom;
//
//	float fDist = D3DXVec3Length(&vDir);
//	float fMinDist = fDist;
//
//	D3DXVec3Normalize(&vDir, &vDir);
//
//	for (size_t buildingNum = 0; buildingNum < m_pBuildings->GetBuilding().size(); buildingNum++)
//	{
//		D3DXVECTOR3 buildingPos = m_pBuildings->GetBuilding()[buildingNum]->GetPosition();
//		D3DXVECTOR3 buildingDir = (buildingPos - m_vFrom);
//
//		if (D3DXVec3Length(&buildingDir) > fDist)
//			continue;
//
//		if (D3DXVec3Dot(&buildingDir, &vDir) < 0)
//			continue;
//
//		D3DXVECTOR3* vertex = m_pBuildings->GetBuilding()[buildingNum]->GetOBB()->GetVertes();
//		int index[24] = { 0,1,2,0,2,3,3,2,6,3,6,7,7,6,5,7,5,4,4,5,1,0,1,3 };
//		for (size_t i = 0; i < 24; i += 3)
//		{
//			float u, v, dist;
//			if (D3DXIntersectTri(&vertex[index[i]], &vertex[index[i + 1]], &vertex[index[i + 2]],
//				&m_vFrom, &vDir, &u, &v, &dist))
//			{
//				if (dist < fMinDist)
//					fMinDist = dist;
//			}
//		}
//	}
//
//	m_vDir = vDir * fMinDist * 0.7f;
//	m_vTo = m_vFrom + m_vDir;
//	m_fDistance = D3DXVec3Length(&m_vDir);
//	m_pTarget->SetDirection(vDir);
//	m_vDir = vDir;
//}

void cStateMove::SetTo(D3DXVECTOR3 vTo)
{
	m_vTo = vTo;
	m_isSetTo = true;
}

void cStateMove::SetPos(D3DXVECTOR3 vTo)
{
	//m_isRandomPos = true;
	//m_fPassedDistance = 0.f;
	//m_pTarget->SetPosition(m_vFrom);

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

		if (D3DXVec3Dot(&buildingDir, &vDir) < 0.5)
			continue;

		D3DXVECTOR3* vertex = m_pBuildings->GetBuilding()[buildingNum]->GetOBB()->GetVertes();
		int index[24] = { 0,1,2,0,2,3,4,6,5,4,7,6,4,5,1,4,1,0,3,2,6,3,6,7 };
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

void cStateMove::ThSetPos(LPVOID pParam)
{
	cStateMove* stateMove = (cStateMove*)pParam;

	EnterCriticalSection(&stateMove->m_CS);
	stateMove->SetPos(stateMove->m_vTo);
	//SetPos(m_vTo);
	LeaveCriticalSection(&stateMove->m_CS);
}
