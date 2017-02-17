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
	m_eStateType = STATE_ATTACK;

	m_vDir = m_vPlayerPos - m_vPosition;
	m_pAI = (cAI*)m_pTarget;
	
	m_pAI->BulletFire(m_vDir);
	m_pAI->SetPosition(m_vPosition);
}

void cStateAttack::Update()
{
}

void cStateAttack::OnStateFinish(cState * pSender)
{
}
