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
#include "cTextMap.h"
#include "cHeightMap.h"
#include "cStaticMesh.h"


cPlayScene::cPlayScene()
	: m_pCamera(NULL)
	, m_pGrid(NULL)
	, m_pTextMap(NULL)
	, m_pHeightMap(NULL)
	, m_pUICursorRoot(NULL)
	, m_pUIPlayerInfoRoot(NULL)
	, m_pPlayer(NULL)
	, m_pSkyView(NULL)
{
}


cPlayScene::~cPlayScene()
{
	if (m_pUICursorRoot)
		m_pUICursorRoot->Destroy();

	if (m_pUIPlayerInfoRoot)
		m_pUIPlayerInfoRoot->Destroy();

	SAFE_RELEASE(m_pSprite);
	SAFE_DELETE(m_pSkyView);

	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pTextMap);
	SAFE_DELETE(m_pHeightMap);

	SAFE_RELEASE(m_pPlayer);
	
	for each(auto p in m_pvAI)
		SAFE_RELEASE(p);

}

void cPlayScene::Setup()
{
	m_pPlayer = new cPlayer;
	m_pPlayer->Setup();

	cAI* pAI = new cAI;
	pAI->Setup("AI/", "AI.X");
	pAI->SetPosition(D3DXVECTOR3(3, 0, 0));
	m_pvAI.push_back(pAI);

	cAI* pAI2 = new cAI;
	pAI2->Setup("AI/", "AI.X");
	pAI2->SetPosition(D3DXVECTOR3(6, 0, 0));
	m_pvAI.push_back(pAI2);


	m_pCamera = new cCamera;
	m_pCamera->Setup(&(m_pPlayer->GetPosition()));

	m_pGrid = new cGrid;
	m_pGrid->Setup();

	m_pTextMap = new cTextMap;
	m_pTextMap->Setup("mapFile.txt");

	m_pHeightMap = new cHeightMap;
	m_pHeightMap->SetupText("Map/","heightMap.txt","Layerstone_512_B_CM.tga");

	m_pSkyView = g_pStaticMeshManager->GetStaticMesh("Map/Sky/", "sky.X");
	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, 0.05f, 0.05f, 0.05f);
	m_pSkyView->SetWorld(matS);

	SettingCursorUI();
	SettingPlayerInfoUI();
}

void cPlayScene::Update()
{
	if (m_pPlayer && m_pCamera)
		m_pPlayer->Update(m_pCamera->GetCamRotAngle()/*, m_pTextMap*/);

	for each(auto p in m_pvAI)
		p->Update(NULL);

	if (m_pTextMap)
		m_pTextMap->Update();

	if (m_pCamera)
		m_pCamera->Update();

	if (m_pUICursorRoot)
		m_pUICursorRoot->Update();

	if (m_pUIPlayerInfoRoot)
		m_pUIPlayerInfoRoot->Update();

	BulletCollisionCheck();
}

void cPlayScene::Render()
{
	if (m_pSkyView)
		m_pSkyView->Render();

	for each(auto p in m_pvAI)
		p->Render();

	if (m_pTextMap)
		m_pTextMap->Render();
	
	if (m_pHeightMap)
		m_pHeightMap->Render();

	if (m_pGrid)
		m_pGrid->Render();

	if (m_pPlayer)
		m_pPlayer->Render();
	
	
	if (m_pUICursorRoot)
		m_pUICursorRoot->Render(m_pSprite);

	if (m_pUIPlayerInfoRoot)
		m_pUIPlayerInfoRoot->Render(m_pSprite);

	if (m_pCompassFront)
		m_pCompassFront->SetAngle(m_pCamera->GetCamRotAngle().y);
}

void cPlayScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
	{
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
	}
}

void cPlayScene::BulletCollisionCheck()
{
	if (m_pPlayer->GetGun() == NULL) return;
	
	cGun* gun = m_pPlayer->GetGun();
	for (size_t aiIndex = 0; aiIndex < m_pvAI.size(); aiIndex++)
	{
		for (size_t bulletIndex = 0; bulletIndex < gun->GetBullets().size(); bulletIndex++)
		{
			D3DXVECTOR3 vBulletCenter, vAICenter;
			vBulletCenter = gun->GetBullets()[bulletIndex]->GetBoundingSphere().vCenter;
			vAICenter = m_pvAI[aiIndex]->GetBoundingSphere().vCenter;

			if (IsCollision(vBulletCenter, BULLET_RADIUS, vAICenter, AI_BOUNDING_SPHERE_SIZE))
			{
				for (size_t sphereIndex = 0; sphereIndex < m_pvAI[aiIndex]->GetBoundingSphereDetail().size(); sphereIndex++)
				{
					if (IsCollision(vBulletCenter, BULLET_RADIUS, m_pvAI[aiIndex]->GetBoundingSphereDetail()[sphereIndex].vCenter, AI_BOUNDING_SPHERE_DETAIL_SIZE))
					{
						SAFE_RELEASE(gun->GetBullets()[bulletIndex]);
						gun->RemoveBullet(bulletIndex);

						if (m_pvAI[aiIndex]->IsAttacked(gun->GetAttackPower()))
						{
							SAFE_RELEASE(m_pvAI[aiIndex]);
							m_pvAI.erase(m_pvAI.begin() + aiIndex);
						}
						return;
					}
				}
			}
		}
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

void cPlayScene::SettingCursorUI()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	float centerX, centerY;
	centerX = (rc.left + rc.right) / 2;
	centerY = (rc.top + rc.bottom) / 2;

	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	m_pUICursorRoot = new cUIObject;
	m_pUICursorRoot->SetPosition(0, 0);

	cUIImageView* pImageCursorL = new cUIImageView;
	pImageCursorL->SetTexture("PlayerUI/cursor_h.tga");
	pImageCursorL->SetPosition(centerX - 25, centerY + 10);

	cUIImageView* pImageCursorR = new cUIImageView;
	pImageCursorR->SetTexture("PlayerUI/cursor_h.tga");
	pImageCursorR->SetPosition(centerX + 35, centerY + 10);

	cUIImageView* pImageCursorT = new cUIImageView;
	pImageCursorT->SetTexture("PlayerUI/cursor_v.tga");
	pImageCursorT->SetPosition(centerX + 7, centerY - 20);

	cUIImageView* pImageCursorB = new cUIImageView;
	pImageCursorB->SetTexture("PlayerUI/cursor_v.tga");
	pImageCursorB->SetPosition(centerX + 7, centerY + 40);

	m_pUICursorRoot->AddChild(pImageCursorL);
	m_pUICursorRoot->AddChild(pImageCursorR);
	m_pUICursorRoot->AddChild(pImageCursorT);
	m_pUICursorRoot->AddChild(pImageCursorB);
}

void cPlayScene::SettingPlayerInfoUI()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	m_pUIPlayerInfoRoot = new cUIObject;
	m_pUIPlayerInfoRoot->SetPosition(0, 0);

	cUIImageView* pHpBack = new cUIImageView;
	pHpBack->SetTexture("PlayerUI/HpBack.png");
	pHpBack->SetPosition(rc.left + pHpBack->GetSize().nWidth / 2 + 20, rc.bottom - pHpBack->GetSize().nHeight / 2);

	cUIImageView* pAmmoBackground = new cUIImageView;
	pAmmoBackground->SetTexture("PlayerUI/Ammo_background.png");
	pAmmoBackground->SetPosition(rc.right - pAmmoBackground->GetSize().nWidth / 2 - 3, rc.bottom - pAmmoBackground->GetSize().nHeight / 2 - 3);

	cUIImageView* pCompassBack = new cUIImageView;
	pCompassBack->SetTexture("PlayerUI/compass_back.png");
	pCompassBack->SetPosition(rc.left + pCompassBack->GetSize().nWidth / 2 + 60, rc.bottom - pHpBack->GetSize().nHeight - pCompassBack->GetSize().nHeight / 2 - 10);

	m_pUIPlayerInfoRoot->AddChild(pHpBack);
	m_pUIPlayerInfoRoot->AddChild(pAmmoBackground);
	m_pUIPlayerInfoRoot->AddChild(pCompassBack);

	// 움직이는 UI
	m_pCompassFront = new cUIImageView;
	m_pCompassFront->SetTexture("PlayerUI/compass_front.png");
	m_pCompassFront->SetPosition(pCompassBack->GetPosition().x, pCompassBack->GetPosition().y);

	m_pUIPlayerInfoRoot->AddChild(m_pCompassFront);
}
