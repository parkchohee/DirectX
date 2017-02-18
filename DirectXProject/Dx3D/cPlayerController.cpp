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
	// angle�� �̿��� direction�� ���Ѵ�. 
	D3DXMATRIXA16 matR, matRX, matRY, matT;
	D3DXMatrixRotationX(&matRX, camAngle.x);
	D3DXMatrixRotationY(&matRY, camAngle.y);
	D3DXVECTOR3 mvDirection = D3DXVECTOR3(0, 0, 1);

	// �̵��ϱ����� Direction, 
	matR = matRY;
	D3DXVec3TransformNormal(&mvDirection, &mvDirection, &matR);

	// �Ѿ� �߻��ϱ����� Direction Y�� �ʿ�
	vDirection = D3DXVECTOR3(0, 0, 1);
	matR = matRX * matRY;
	D3DXVec3TransformNormal(&vDirection, &vDirection, &matR);


	D3DXVECTOR3 _vPosition = vPosition;

	// direction �������� ������,
	if (g_pKeyManager->IsStayKeyDown('W'))			// ������ ������
	{
		_vPosition = vPosition + (mvDirection * m_fMoveSpeed);
	}
	else if (g_pKeyManager->IsStayKeyDown('S'))		// �ڷ� ������
	{
		_vPosition = vPosition - (mvDirection * m_fMoveSpeed);
	}

	D3DXMatrixRotationY(&matR, camAngle.y - D3DX_PI / 2);
	mvDirection = D3DXVECTOR3(0, 0, 1);
	D3DXVec3TransformNormal(&mvDirection, &mvDirection, &matR);
	
	if (g_pKeyManager->IsStayKeyDown('A'))			// �������� ������
	{
		_vPosition = vPosition + (mvDirection * m_fMoveSpeed);
	}
	else if (g_pKeyManager->IsStayKeyDown('D'))		// ���������� ������
	{
		_vPosition = vPosition - (mvDirection * m_fMoveSpeed);
	}

	if (m_pTextMap)
	{
		// ���� �ǹ����� obb�� �ҷ��� obb �浹üũ
		for (size_t i = 0; i < m_pTextMap->GetBuildings().size(); i++)
		{
			// �浹�ϸ� �׳� ����
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
