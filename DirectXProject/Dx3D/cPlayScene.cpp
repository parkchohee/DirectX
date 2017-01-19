#pragma once

#include "stdafx.h"
#include "cPlayScene.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cUIImageView.h"
#include "cPlayer.h"
#include "cAI.h"
#include "cGun.h"
#include "cBullet.h"
#include "cOBB.h"


cPlayScene::cPlayScene()
	: m_pCamera(NULL)
	, m_pGrid(NULL)
	, m_pUIRoot(NULL)
	, m_pPlayer(NULL)
	, m_pAI(NULL)
{
}


cPlayScene::~cPlayScene()
{
	if (m_pUIRoot)
		m_pUIRoot->Destroy();

	SAFE_RELEASE(m_pSprite);

	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);

	SAFE_RELEASE(m_pPlayer);
	SAFE_RELEASE(m_pAI);

}

void cPlayScene::Setup()
{
	m_pPlayer = new cPlayer;
	m_pPlayer->Setup();

	m_pAI = new cAI;
	m_pAI->SetPosition(D3DXVECTOR3(3, 0, 0));
	m_pAI->Setup("AI/", "testMan.X");

	m_pCamera = new cCamera;
//	m_pCamera->Setup(NULL);
	m_pCamera->Setup(&(m_pPlayer->GetPosition()));

	m_pGrid = new cGrid;
	m_pGrid->Setup();

	/// >> : cursor UI
	{
		RECT rc;
		GetClientRect(g_hWnd, &rc);

		float centerX, centerY;
		centerX = (rc.left + rc.right) / 2;
		centerY = (rc.top + rc.bottom) / 2;

		D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

		m_pUIRoot = new cUIObject;
		m_pUIRoot->SetPosition(centerX, centerY);

		cUIImageView* pImageCursorL = new cUIImageView;
		pImageCursorL->SetTexture("PlayerUI/cursor_h.tga");
		pImageCursorL->SetPosition(-16, 7);

		cUIImageView* pImageCursorR = new cUIImageView;
		pImageCursorR->SetTexture("PlayerUI/cursor_h.tga");
		pImageCursorR->SetPosition(16, 7);

		cUIImageView* pImageCursorT = new cUIImageView;
		pImageCursorT->SetTexture("PlayerUI/cursor_v.tga");
		pImageCursorT->SetPosition(7, -16);

		cUIImageView* pImageCursorB = new cUIImageView;
		pImageCursorB->SetTexture("PlayerUI/cursor_v.tga");
		pImageCursorB->SetPosition(7, 16);

		m_pUIRoot->AddChild(pImageCursorL);
		m_pUIRoot->AddChild(pImageCursorR);
		m_pUIRoot->AddChild(pImageCursorT);
		m_pUIRoot->AddChild(pImageCursorB);
	}
}

void cPlayScene::Update()
{
	if (m_pPlayer && m_pCamera)
		m_pPlayer->Update(m_pCamera->GetCamRotAngle());

	if (m_pAI)
		m_pAI->Update(NULL);

	if (m_pCamera)
		m_pCamera->Update();

	if (m_pUIRoot)
		m_pUIRoot->Update();

	if (m_pPlayer->GetGun())
	{
		cGun* gun = m_pPlayer->GetGun();

		for (size_t bulletIndex = 0; bulletIndex < gun->GetBullets().size(); bulletIndex++)
		{
			D3DXVECTOR3 vBulletCenter, vAICenter;
			vBulletCenter = gun->GetBullets()[bulletIndex]->GetBoundingSphere().vCenter;
			vAICenter = m_pAI->GetBoundingSphere().vCenter;

			if (IsCollision(vBulletCenter, BULLET_RADIUS, vAICenter, AI_BOUNDING_SPHERE_SIZE))
			{
				for (size_t sphereIndex = 0; sphereIndex < m_pAI->GetBoundingSphereDetail().size(); sphereIndex++)
				{
					if (IsCollision(vBulletCenter, BULLET_RADIUS, m_pAI->GetBoundingSphereDetail()[sphereIndex].vCenter, AI_BOUNDING_SPHERE_DETAIL_SIZE))
					{
						int a = 0;
					}
				}
			}

			/*for (size_t j = 0; j < m_pAI->GetBoundingSphere().size(); j++)
			{
				D3DXVECTOR3 BulletPos, CrushManPos;
				BulletPos = gun->GetBullets()[i]->GetBoundingSphere().vCenter;
				CrushManPos = m_pAI->GetBoundingSphere()[j].vCenter;

				if (IsCollision(BulletPos, 0.1f, CrushManPos, 0.2f))
				{
					int a = 0;
				}
			}*/
		}
	}

}

void cPlayScene::Render()
{
	if (m_pAI)
		m_pAI->Render();

	if (m_pPlayer)
		m_pPlayer->Render();
	
	if (m_pGrid)
		m_pGrid->Render();

	if (m_pUIRoot)
		m_pUIRoot->Render(m_pSprite);
}

void cPlayScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//if (m_pPlayer)
	//{
	//	m_pPlayer->WndProc(hWnd, message, wParam, lParam);
	//}

	if (m_pCamera)
	{
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
	}

}

float cPlayScene::GetDistance(D3DXVECTOR3 BulletPos, D3DXVECTOR3 CrushManPos)
{
	return ((CrushManPos.x - BulletPos.x)*(CrushManPos.x - BulletPos.x)) +
		((CrushManPos.y - BulletPos.y)*(CrushManPos.y - BulletPos.y)) +
		((CrushManPos.z - BulletPos.z)* (CrushManPos.z - BulletPos.z));
}

bool cPlayScene::IsCollision(D3DXVECTOR3 BulletPos, float BulletSphereRadius, D3DXVECTOR3 CrushManPos, float CrushManSphereRadius)
{
	return GetDistance(BulletPos, CrushManPos) < ((BulletSphereRadius + CrushManSphereRadius) * (BulletSphereRadius + CrushManSphereRadius));
}
