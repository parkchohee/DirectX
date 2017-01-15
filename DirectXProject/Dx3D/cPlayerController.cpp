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

	// direction 방향으로 앞으로,
	if (GetKeyState('W') & 0x8000)			// 앞으로 움직임
	{
		vPosition += (mvDirection * 0.1f);
	}
	else if (GetKeyState('S') & 0x8000)		// 뒤로 움직임
	{
		vPosition -= (mvDirection * 0.1f);
	}

	D3DXMatrixRotationY(&matR, camAngle.y - D3DX_PI / 2);
	mvDirection = D3DXVECTOR3(0, 0, 1);
	D3DXVec3TransformNormal(&mvDirection, &mvDirection, &matR);
	
	if (GetKeyState('A') & 0x8000)			// 왼쪽으로 움직임
	{
		vPosition += (mvDirection * 0.1f);
	}
	else if (GetKeyState('D') & 0x8000)		// 오른쪽으로 움직임
	{
		vPosition -= (mvDirection * 0.1f);
	}
	
}
