#pragma once

#include "stdafx.h"
#include "cPlayScene.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cUIImageView.h"
#include "cUITextView.h"
#include "cPlayer.h"
#include "cAI.h"
#include "cGun.h"
#include "cBullet.h"
#include "cOBB.h"
#include "cTextMap.h"
#include "cHeightMap.h"
#include "cStaticMesh.h"
#include "cRay.h"


#include "cState.h"
#include "cStateMove.h"


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
	
	for each(auto p in m_pvDeathAI)
		SAFE_RELEASE(p);
}

void cPlayScene::Setup()
{
	m_pTextMap = new cTextMap;
	m_pTextMap->Setup("mapFile.txt");

	m_pHeightMap = new cHeightMap;
	m_pHeightMap->SetupText("Map/", "heightMap.txt", "Ground_CMGround_CM.tga");

	m_pPlayer = new cPlayer;
	m_pPlayer->Setup();
	m_pPlayer->SetHeightMap(m_pHeightMap);
	m_pPlayer->SetTextMap(m_pTextMap);

	for (int i = 0; i < 10; i++)
	{
		cAI* pAI = new cAI;
		pAI->Setup("AI/", "AI.X");
		pAI->SetPosition(D3DXVECTOR3(rand() % 10, 0, rand() % 10));
		pAI->SetHeightMap(m_pHeightMap);
		pAI->SetTextMap(m_pTextMap);
		pAI->SetIsEnemy(true);		// true이면 적
	

		cStateMove* pStateMove = new cStateMove;
		pStateMove->SetFrom(pAI->GetPosition());
		pStateMove->SetTarget(pAI);
		pStateMove->SetTo(D3DXVECTOR3(rand() % 10, 0, rand() % 10));
		pStateMove->Start();
		pStateMove->SetDelegate(pStateMove);

		pAI->SetState(pStateMove);
		SAFE_RELEASE(pStateMove);

		m_pvAI.push_back(pAI);
	}

	m_pCamera = new cCamera;
	m_pCamera->Setup(&(m_pPlayer->GetPosition()));
	//m_pCamera->Setup(NULL);

	m_pGrid = new cGrid;
	m_pGrid->Setup();

	
	m_pSkyView = g_pStaticMeshManager->GetStaticMesh("Map/Sky/", "sky.X");
	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, 0.05f, 0.05f, 0.05f);
	m_pSkyView->SetWorld(matS);

	SettingCursorUI();
	SettingPlayerInfoUI();

}

void cPlayScene::Update()
{
	if (m_pPlayer == NULL) return;

	if (m_pCamera)
		m_pPlayer->Update(m_pCamera->GetCamRotAngle());

	for each(auto p in m_pvAI)
		p->Update(m_pPlayer->GetPosition(), m_pCamera->GetCamRotAngle().y);


	for (size_t i = 0; i < m_pvDeathAI.size(); )
	{
		if (m_pvDeathAI[i]->IsDeath())
		{
			SAFE_RELEASE(m_pvDeathAI[i]);
			m_pvDeathAI.erase(m_pvDeathAI.begin() + i);
			continue;
		}
		
		i++;
	}

	if (m_pTextMap)
		m_pTextMap->Update();

	if (m_pCamera)
		m_pCamera->Update();

	if (m_pUICursorRoot)
		m_pUICursorRoot->Update();

	if (m_pBulletText)
		m_pBulletText->SetText(std::to_string(m_pPlayer->GetGun()->GetCurrentBullet()) 
			+ "/" + std::to_string(m_pPlayer->GetGun()->GetMaxBullet()));
	

	if (m_pUIPlayerInfoRoot)
		m_pUIPlayerInfoRoot->Update();

	if (g_pKeyManager->IsOnceKeyDown(VK_LBUTTON))
		PlayerBulletFire();

}

void cPlayScene::Render()
{
	if (m_pSkyView)
		m_pSkyView->Render();

	for each(auto p in m_pvAI)
		p->Render();
	
	for each(auto p in m_pvDeathAI)
		p->Render();

	if (m_pHeightMap)
		m_pHeightMap->Render();

	if (m_pTextMap)
		m_pTextMap->Render();

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

void cPlayScene::PlayerBulletFire()
{
	if (m_pPlayer == NULL) return;
	if (m_pPlayer->GetGun() == NULL) return;
	
	float centerX, centerY;
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	centerX = (rc.left + rc.right) / 2;
	centerY = (rc.top + rc.bottom) / 2;

	cRay r = cRay::RayAtWorldSpace(centerX, centerY);
	
	m_pPlayer->BulletFire(r.GetRayDir());

	// sort 알고리즘 돌리면 계산량 증가 -> distance 계산, sort, 스피어충돌 모두 계산해야함.
	// 스피어충돌, 충돌한것만 dist계산 하도록 하여 계산량 줄임.

	// gun 의 사정거리 dist를 놓고 (dist 는 제곱값.. sqrt 연산량 줄이기 위해)
	// 디테일 스피어 충돌하면 dist 최소값으로 업데이트
	// dist가 최소가 나오는 ai의 index저장하고
	// ai index를 다 돌면 dist 의 값이 변경 된 경우 
	// 저장해 놓은 index번쨰의 ai 죽임.

	cGun* gun = m_pPlayer->GetGun();
	float fAttackRange = gun->GetAttackRange() * gun->GetAttackRange();
	float fMinDist = fAttackRange + 0.0001f;
	int nMinDistAiIndex = 0;

	for (size_t aiIndex = 0; aiIndex < m_pvAI.size(); aiIndex++)
	{
		// 우리팀은 건너뛴다.
		if (!m_pvAI[aiIndex]->GetIsEnemy()) continue;

		// 상대팀이면 충돌체크
		if (!r.IsPicked(&m_pvAI[aiIndex]->GetBoundingSphere()))
			continue;

		for (size_t sphereIndex = 0; sphereIndex < m_pvAI[aiIndex]->GetBoundingSphereDetail().size(); sphereIndex++)
		{
			if (r.IsPicked(&m_pvAI[aiIndex]->GetBoundingSphereDetail()[sphereIndex]))
			{
				float dist = GetDistance(m_pPlayer->GetPosition(), m_pvAI[aiIndex]->GetBoundingSphereDetail()[sphereIndex].vCenter);
				if (fMinDist > dist)
				{
					fMinDist = dist;
					nMinDistAiIndex = aiIndex;
				}
			}
		}
		/*for (size_t bulletIndex = 0; bulletIndex < gun->GetBullets().size(); bulletIndex++)
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
		}*/
	}
	

	if (fAttackRange > fMinDist)
	{
		m_pvAI[nMinDistAiIndex]->Destroy();
		m_pvDeathAI.push_back(m_pvAI[nMinDistAiIndex]);
		m_pvAI.erase(m_pvAI.begin() + nMinDistAiIndex);
	}


}

float cPlayScene::GetDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	return ((pos2.x - pos1.x)*(pos2.x - pos1.x)) +
		((pos2.y - pos1.y)*(pos2.y - pos1.y)) +
		((pos2.z - pos1.z)* (pos2.z - pos1.z));
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

	// 텍스트 UI
	m_pBulletText = new cUITextView;
	m_pBulletText->SetText("0/0");
	m_pBulletText->SetSize(ST_SIZEN(200, 100));
	m_pBulletText->SetPosition(pAmmoBackground->GetPosition().x, pAmmoBackground->GetPosition().y);

	m_pUIPlayerInfoRoot->AddChild(m_pBulletText);

	// 움직이는 UI
	m_pCompassFront = new cUIImageView;
	m_pCompassFront->SetTexture("PlayerUI/compass_front.png");
	m_pCompassFront->SetPosition(pCompassBack->GetPosition().x, pCompassBack->GetPosition().y);

	m_pUIPlayerInfoRoot->AddChild(m_pCompassFront);
}
