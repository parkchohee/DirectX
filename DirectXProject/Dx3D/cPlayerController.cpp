#include "stdafx.h"
#include "cPlayerController.h"


cPlayerController::cPlayerController()
{
}


cPlayerController::~cPlayerController()
{
}

void cPlayerController::Setup()
{
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

	// direction �������� ������,
	if (GetKeyState('W') & 0x8000)			// ������ ������
	{
		vPosition += (mvDirection * 0.1f);
	}
	else if (GetKeyState('S') & 0x8000)		// �ڷ� ������
	{
		vPosition -= (mvDirection * 0.1f);
	}

	D3DXMatrixRotationY(&matR, camAngle.y - D3DX_PI / 2);
	mvDirection = D3DXVECTOR3(0, 0, 1);
	D3DXVec3TransformNormal(&mvDirection, &mvDirection, &matR);
	
	if (GetKeyState('A') & 0x8000)			// �������� ������
	{
		vPosition += (mvDirection * 0.1f);
	}
	else if (GetKeyState('D') & 0x8000)		// ���������� ������
	{
		vPosition -= (mvDirection * 0.1f);
	}
	
}
