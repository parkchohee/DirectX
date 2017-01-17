#include "stdafx.h"
#include "cAIController.h"


cAIController::cAIController()
{
}


cAIController::~cAIController()
{
}

void cAIController::Setup(float moveSpeed)
{
	m_fMoveSpeed = moveSpeed;
}

void cAIController::Update(D3DXVECTOR3 & camAngle, OUT D3DXVECTOR3 & vDirection, OUT D3DXVECTOR3 & vPosition)
{
}
