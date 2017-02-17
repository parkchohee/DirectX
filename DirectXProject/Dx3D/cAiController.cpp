#include "stdafx.h"
#include "cAIController.h"
#include "cTextMap.h"
#include "cHeightMap.h"
#include "cBuilding.h"
#include "cOBB.h"
#include "cAI.h"

#include "cGun.h"

#include "cState.h"
#include "cStateMove.h"
#include "cStateAttack.h"


cAIController::cAIController()
	: m_fAttackRange(20.0f)
	, m_pTarget(NULL)
{
}


cAIController::~cAIController()
{
}

void cAIController::Setup(float moveSpeed, cCharacter* pCharacter)
{
	m_fMoveSpeed = moveSpeed;

	m_pTarget = (cAI*)pCharacter;

	if (m_pTarget)
	{
		cStateMove* pStateMove = new cStateMove;
		pStateMove->SetFrom(m_pTarget->GetPosition());
		pStateMove->SetTarget(m_pTarget);
		pStateMove->SetTo(D3DXVECTOR3(10, 0, 0));
		pStateMove->Start();
		pStateMove->SetTextMap(m_pTextMap);
		pStateMove->SetDelegate(pStateMove);

		m_pTarget->SetState(pStateMove);
		SAFE_RELEASE(pStateMove);

		m_fAttackRange = m_pTarget->GetGun()->GetAttackRange();
	}
}

void cAIController::Update(OUT D3DXVECTOR3 & vPlayer, OUT D3DXVECTOR3 & vDirection, OUT D3DXVECTOR3 & vPosition)
{
	if (m_pTarget == NULL) return;
	
	D3DXVECTOR3 t = m_pTarget->GetPosition();
	float f = D3DXVec3Length(&(vPlayer - t));
	

	if (f < m_fAttackRange + 5)	// player가 탐색 거리 안에 들어옴
	{
		if (f < m_fAttackRange)	// 공격 사정거리 안에 들어옴
		{
			if (m_pTarget)
			{
				if (m_pTarget->GetState()->GetStateType() == STATE_ATTACK)
					return;

				D3DXVECTOR3 vDir;
				vDir = m_pTarget->GetPosition() - vPlayer;

				// attack state를 만듬..
				cStateAttack* pStateAttack = new cStateAttack;
				pStateAttack->SetTarget(m_pTarget);
				pStateAttack->SetPosition(m_pTarget->GetPosition());
				pStateAttack->SetPlayerPos(vPlayer);
				pStateAttack->Start();
			
				m_pTarget->SetState(pStateAttack);
				SAFE_RELEASE(pStateAttack);
			}
		}
		else
		{
			if (m_pTarget)
			{
				D3DXVECTOR3 vTo, vDir;
				vDir = vPlayer - m_pTarget->GetPosition();

				float vLength = D3DXVec3Length(&vDir) - 5.0f;

				D3DXVec3Normalize(&vDir, &vDir);
				vTo = m_pTarget->GetPosition() + vDir * vLength;

				cStateMove* pStateMove = new cStateMove;
				pStateMove->SetFrom(m_pTarget->GetPosition());
				pStateMove->SetTarget(m_pTarget);
				pStateMove->SetTo(vTo);
				pStateMove->Start();
				pStateMove->SetTextMap(m_pTextMap);
				pStateMove->SetDelegate(pStateMove);

				m_pTarget->SetState(pStateMove);
				SAFE_RELEASE(pStateMove);
			}
		}
		
	}

}
