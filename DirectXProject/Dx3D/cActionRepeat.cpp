#include "StdAfx.h"
#include "cActionRepeat.h"


cActionRepeat::cActionRepeat(void)
	: m_pAction(NULL)
{
}


cActionRepeat::~cActionRepeat(void)
{
	SAFE_RELEASE(m_pAction);
}

void cActionRepeat::Start()
{
	if(m_pAction == NULL)
		return;
	m_pAction->SetDelegate(this);
	m_pAction->Start();
}

void cActionRepeat::Update()
{
	if(m_pAction == NULL)
		return;
	m_pAction->Update();
}

void cActionRepeat::OnActionFinish( cAction* pSender )
{
	m_pAction->Start();
}
