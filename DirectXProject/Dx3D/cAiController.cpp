#include "stdafx.h"
#include "cAIController.h"


cAIController::cAIController()
	: m_fAngleX(0.0f)
	, m_fAngleY(0.0f)
{
}


cAIController::~cAIController()
{
}

void cAIController::Setup(float moveSpeed)
{
	m_fMoveSpeed = moveSpeed;
}

void cAIController::Update(OUT D3DXVECTOR3 & rotateAngle, OUT D3DXVECTOR3 & vDirection, OUT D3DXVECTOR3 & vPosition)
{
	if (g_pKeyManager->IsStayKeyDown(VK_LEFT))
		m_fAngleY -= 0.1f;
	if (g_pKeyManager->IsStayKeyDown(VK_RIGHT))
		m_fAngleY += 0.1f;

	rotateAngle.x = m_fAngleX;
	rotateAngle.y = m_fAngleY;


	D3DXMATRIXA16 matRX, matRY;
	D3DXMatrixRotationX(&matRX, m_fAngleX);
	D3DXMatrixRotationY(&matRY, m_fAngleY);
	
	D3DXVECTOR3 mvDirection = D3DXVECTOR3(0, 0, 1);
	D3DXVec3TransformCoord(&mvDirection, &mvDirection, &matRY);

	if (g_pKeyManager->IsStayKeyDown(VK_UP))
		vPosition += (mvDirection * m_fMoveSpeed);
	if (g_pKeyManager->IsStayKeyDown(VK_DOWN))
		vPosition -= (mvDirection * m_fMoveSpeed);

}
