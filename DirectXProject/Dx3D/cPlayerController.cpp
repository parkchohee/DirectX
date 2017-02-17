#include "stdafx.h"
#include "cPlayerController.h"
#include "cHeightMap.h"
#include "cTextMap.h"
#include "cOBB.h"
#include "cBuilding.h"


cPlayerController::cPlayerController()
{
}


cPlayerController::~cPlayerController()
{
}

void cPlayerController::Setup(float moveSpeed, cCharacter* pCharacter)
{
	m_fMoveSpeed = moveSpeed;
}

void cPlayerController::Update(D3DXVECTOR3 & camAngle, OUT D3DXVECTOR3 & vDirection, OUT D3DXVECTOR3 & vPosition)
{
	// angle을 이용해 direction을 구한다. 
	D3DXMATRIXA16 matR, matRX, matRY, matT;
	D3DXMatrixRotationX(&matRX, camAngle.x);
	D3DXMatrixRotationY(&matRY, camAngle.y);
	D3DXVECTOR3 mvDirection = D3DXVECTOR3(0, 0, 1);

	// 이동하기위한 Direction, 
	matR = matRY;
	D3DXVec3TransformNormal(&mvDirection, &mvDirection, &matR);

	// 총알 발사하기위한 Direction Y값 필요
	vDirection = D3DXVECTOR3(0, 0, 1);
	matR = matRX * matRY;
	D3DXVec3TransformNormal(&vDirection, &vDirection, &matR);


	D3DXVECTOR3 _vPosition = vPosition;

	// direction 방향으로 앞으로,
	if (g_pKeyManager->IsStayKeyDown('W'))			// 앞으로 움직임
	{
		_vPosition = vPosition + (mvDirection * m_fMoveSpeed);
	}
	else if (g_pKeyManager->IsStayKeyDown('S'))		// 뒤로 움직임
	{
		_vPosition = vPosition - (mvDirection * m_fMoveSpeed);
	}

	D3DXMatrixRotationY(&matR, camAngle.y - D3DX_PI / 2);
	mvDirection = D3DXVECTOR3(0, 0, 1);
	D3DXVec3TransformNormal(&mvDirection, &mvDirection, &matR);
	
	if (g_pKeyManager->IsStayKeyDown('A'))			// 왼쪽으로 움직임
	{
		_vPosition = vPosition + (mvDirection * m_fMoveSpeed);
	}
	else if (g_pKeyManager->IsStayKeyDown('D'))		// 오른쪽으로 움직임
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
