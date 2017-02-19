#include "stdafx.h"
#include "cStateAttack.h"

#include "cAI.h"

cStateAttack::cStateAttack()
{
}


cStateAttack::~cStateAttack()
{
}

void cStateAttack::Start()
{
	//if (m_pTarget)
	//{
	//	m_eStateType = STATE_ATTACK;

	//	m_pAI = (cAI*)m_pTarget;

	//	D3DXVec3Normalize(&m_vDir, &m_vDir);

	//	m_pAI->BulletFire(m_vDir);
	//	m_pAI->SetPosition(m_vPosition);
	//}
}

void cStateAttack::Update()
{
	/*if(m_pTarget)*/
}

void cStateAttack::OnStateFinish(cState * pSender)
{
}
