#include "stdafx.h"
#include "cAIController.h"
#include "cTextMap.h"
#include "cHeightMap.h"
#include "cBuilding.h"
#include "cOBB.h"

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


	D3DXVECTOR3 _vPosition = vPosition;

	if (g_pKeyManager->IsStayKeyDown(VK_UP))
	{
		_vPosition = vPosition + (mvDirection * m_fMoveSpeed);
	}
	else if (g_pKeyManager->IsStayKeyDown(VK_DOWN))
	{
		_vPosition = vPosition - (mvDirection * m_fMoveSpeed);
	}

	if (m_pTextMap)
	{
		// 맵의 건물들의 obb를 불러와 obb 충돌체크
		for (size_t i = 0; i < m_pTextMap->GetBuildings().size(); i++)
		{
			// 충돌하면 그냥 리턴
			if (cOBB::IsCollision(m_pTextMap->GetBuildings()[i]->GetOBB(), m_pOBB))
				return;
		}
	}

	if (m_pHeightMap)
	{
		if (m_pHeightMap->GetHeight(_vPosition.x, _vPosition.y, _vPosition.z))
		{
			vPosition = _vPosition;
		}
	}
	
}
