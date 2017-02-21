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
#include "cRay.h"
#include "cBuildingGroup.h"
#include "cEvent.h"
#include "cAirDrop.h"
#include "cFrustum.h"

cPlayScene::cPlayScene()
	: m_pCamera(NULL)
	, m_pGrid(NULL)
	, m_pTextMap(NULL)
	, m_pHeightMap(NULL)
	, m_pPlayer(NULL)
	, m_pSkyView(NULL)
	, m_pEvent(NULL)
	, m_pAirDrop(NULL)
	, m_eState(AIRDROP_STATE)
	, m_ePrevState(AIRDROP_STATE)
	, m_pFrustum(NULL)
	, m_pQuitGameRoot(NULL)
{
}


cPlayScene::~cPlayScene()
{
}

void cPlayScene::Setup()
{
	m_ePrevState = m_eState = AIRDROP_STATE;

	m_pTextMap = new cTextMap;
	m_pTextMap->Setup("mapFile.txt");

	SettingBuildingGroup();

	m_pHeightMap = new cHeightMap;
	m_pHeightMap->SetupText("Map/", "heightMap.txt", "Ground_CMGround_CM.tga");

	m_pPlayer = new cPlayer;
	m_pPlayer->Setup();
	m_pPlayer->SetBuildings(m_pvBuildingGroup[0]);
	m_pPlayer->SetHeightMap(m_pHeightMap);
	m_pPlayer->SetTextMap(m_pTextMap);
	m_pPlayer->SetMaxHp(1000);
	m_pPlayer->SetCurrentHp(1000);

	for (int i = 0; i < 10; i++)
	{
		cAI* pAI = new cAI;
		pAI->SetBuildings(m_pvBuildingGroup[0]);
		pAI->SetPosition(D3DXVECTOR3(-20, 0, 20));
		pAI->Setup("AI/", "soldier.X");
		pAI->SetHeightMap(m_pHeightMap);
		pAI->SetTextMap(m_pTextMap);
		m_pvAI.push_back(pAI);
	}

	for (int i = 0; i < 10; i++)
	{
		cAI* pAI = new cAI;
		pAI->SetBuildings(m_pvBuildingGroup[1]);
		pAI->SetPosition(D3DXVECTOR3(20, 0, 20));
		pAI->Setup("AI/", "soldier.X");
		pAI->SetHeightMap(m_pHeightMap);
		pAI->SetTextMap(m_pTextMap);
		m_pvAI.push_back(pAI);
	}

	for (int i = 0; i < 10; i++)
	{
		cAI* pAI = new cAI;
		pAI->SetBuildings(m_pvBuildingGroup[2]);
		pAI->SetPosition(D3DXVECTOR3(-20, 0, -20));
		pAI->Setup("AI/", "soldier.X");
		pAI->SetHeightMap(m_pHeightMap);
		pAI->SetTextMap(m_pTextMap);
		m_pvAI.push_back(pAI);
	}

	for (int i = 0; i < 10; i++)
	{
		cAI* pAI = new cAI;
		pAI->SetBuildings(m_pvBuildingGroup[3]);
		pAI->SetPosition(D3DXVECTOR3(20, 0, -20));
		pAI->Setup("AI/", "soldier.X");
		pAI->SetHeightMap(m_pHeightMap);
		pAI->SetTextMap(m_pTextMap);
		m_pvAI.push_back(pAI);
	}

	m_pCamera = new cCamera;
	m_pCamera->Setup(&(m_pPlayer->GetPosition()));
	//m_pCamera->Setup(NULL);

	m_pGrid = new cGrid;
	m_pGrid->Setup(50,1.0f);

	
	m_pSkyView = new cStaticMesh("Map/Sky/", "sky.X");
	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, 0.05f, 0.05f, 0.05f);
	m_pSkyView->SetWorld(matS);

	m_pAirDrop = new cAirDrop;
	m_pAirDrop->Setup(m_pHeightMap);
	m_pCamera->Setup(&m_pAirDrop->GetPosition());

	m_pFrustum = new cFrustum;
	m_pFrustum->Setup();

	QuitGameUISetting();
}

void cPlayScene::Destroy()
{
	if (m_pQuitGameRoot)
		m_pQuitGameRoot->Destroy();

	SAFE_RELEASE(m_pSprite);
	SAFE_RELEASE(m_pEvent);
	SAFE_DELETE(m_pFrustum);
	SAFE_RELEASE(m_pAirDrop);
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

	for each(auto p in m_pvBuildingGroup)
		SAFE_RELEASE(p);

	for each(auto p in m_pAttackEvent)
		SAFE_RELEASE(p);

	m_pvAI.clear();
	m_pvDeathAI.clear();
	m_pvBuildingGroup.clear();
	m_pAttackEvent.clear();
}

void cPlayScene::Update()
{
	if (m_pPlayer == NULL) return;

	if (m_eState == PAUSE_STATE)
	{
		if (m_pQuitGameRoot)
			m_pQuitGameRoot->Update();
		return;
	}

	if (g_pKeyManager->IsOnceKeyDown(VK_ESCAPE))
	{
		if (m_eState == PAUSE_STATE)
			m_eState = m_ePrevState;
		else
			m_eState = PAUSE_STATE;
	}

	UpdatePlayerBuildingGroup();

	if (m_pCamera)
		m_pPlayer->Update(m_pCamera->GetCamRotAngle());

	if (m_pTextMap)
		m_pTextMap->Update();

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


	if (m_pEvent)
	{
		m_pEvent->Update();

		if (!m_pEvent->GetIsPlay())
		{
			SAFE_RELEASE(m_pEvent);

			if (m_eState == GAME_OVER)
			{
				g_pSceneManager->ChangeSceneWithLoading("firstScene", "loadingScene");
			}
		}
	}

	if (m_pCamera)
		m_pCamera->Update();

	if ((m_eState != GAME_OVER))
	{
		if (m_eState != AIRDROP_STATE)
		{
			PlayerBulletCollision();
			AIBulletCollision();
		}
		else
		{
			if (m_pAirDrop)
			{
				m_pAirDrop->Update();

				if (!m_pAirDrop->isPlay())
				{
					m_eState = NORMAL_STATE;
					m_pCamera->Setup(&(m_pPlayer->GetPosition()));
				}
			}
		}
	}


	for (size_t i = 0; i < m_pAttackEvent.size(); )
	{
		if (!m_pAttackEvent[i]->GetIsPlay())
		{
			SAFE_RELEASE(m_pAttackEvent[i]);
			m_pAttackEvent.erase(m_pAttackEvent.begin() + i);
			continue;
		}

		m_pAttackEvent[i]->Update();
		i++;
	}

	m_pFrustum->Update();
}

void cPlayScene::Render()
{
	if (m_pSkyView)
		m_pSkyView->Render();

	if (m_pHeightMap)
		m_pHeightMap->Render();

	if (m_pTextMap)
		m_pTextMap->Render();

	//if (m_pGrid)
	//	m_pGrid->Render();

	for each(auto p in m_pvAI)
	{
		if (m_pFrustum->IsIn(p->GetPosition()))
			p->Render();

		p->SpriteRender();
	}

	for each(auto p in m_pvDeathAI)
		p->Render();

	for (size_t i = 0; i < m_pAttackEvent.size(); i++)
		m_pAttackEvent[i]->Render();

	if (m_pEvent)
		m_pEvent->Render();
	
	if (m_eState != AIRDROP_STATE)
	{
		if (m_pPlayer)
			m_pPlayer->Render();
	}
	
	if (m_eState == AIRDROP_STATE)
	{
		if (m_pAirDrop)
			m_pAirDrop->Render();
	}
	else if (m_eState == PAUSE_STATE)
	{
		if (m_pQuitGameRoot)
			m_pQuitGameRoot->Render(m_pSprite);
	}
	
}

void cPlayScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
	{
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
	}
}

void cPlayScene::PlayerBulletCollision()
{
	if (m_pPlayer == NULL) return;
	if (m_pPlayer->GetGun() == NULL) return;
	if (m_pPlayer->GetBullet() == NULL) return;

	m_pCamera->SetShakePow(0.5f);

	cRay r = m_pPlayer->GetBullet()->RayAtWorldSpace(CENTERX, CENTERY);

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
	}

	if (fAttackRange > fMinDist)
	{
		m_pPlayer->GetGun()->SetCurrentExp(m_pPlayer->GetGun()->GetCurrentExp() + 1.0f);
		LevUpCheck();

		if (m_pvAI[nMinDistAiIndex]->IsAttacked(m_pPlayer->GetGun()->GetAttackPower())) // 피없으면 죽임
		{
			m_pvAI[nMinDistAiIndex]->Destroy();
			m_pvDeathAI.push_back(m_pvAI[nMinDistAiIndex]);
			m_pvAI.erase(m_pvAI.begin() + nMinDistAiIndex);
		}
	}

	// 안맞았으면 ray 삭제
	m_pPlayer->DeleteRay();
}

void cPlayScene::AIBulletCollision()
{
	D3DXVECTOR3 vPlayerPos = m_pPlayer->GetPosition();

	for (size_t aiIndex = 0; aiIndex < m_pvAI.size(); aiIndex++)
	{
		cGun* gun = m_pvAI[aiIndex]->GetGun();

		for (size_t bulletIndex = 0; bulletIndex < gun->GetBullets().size(); bulletIndex++)
		{
			D3DXVECTOR3 vBulletCenter;
			vBulletCenter = gun->GetBullets()[bulletIndex]->GetBoundingSphere().vCenter;

			if (IsCollision(vBulletCenter, BULLET_RADIUS, vPlayerPos, PLAYER_BOUNDING_SPHERE_SIZE))
			{
				D3DXVECTOR3 vDir = m_pvAI[aiIndex]->GetPosition() - vPlayerPos;
				D3DXVec3Normalize(&vDir, &vDir);

				float angle = acos(D3DXVec3Dot(&vDir, &m_pPlayer->GetDirection()));

				D3DXVECTOR3 vRightDir = m_pPlayer->GetDirection();
				D3DXMATRIXA16 matR;
				D3DXMatrixRotationY(&matR, D3DX_PI / 2);
				D3DXVec3TransformCoord(&vRightDir, &vRightDir, &matR);
				D3DXVec3Normalize(&vRightDir, &vRightDir);

				if (D3DXVec3Dot(&vDir, &vRightDir) < 0)
					angle = -angle;

				cEvent* attackEvent = new cEvent;
				attackEvent->Setup("PlayerUI/hud_hitIndicator.tga",
					"PlayerUI/hud_hitIndicator.tga", 30, 90);
			
				attackEvent->SetAngle(angle);
				
				m_pAttackEvent.push_back(attackEvent);


				if (m_pPlayer->IsAttacked(gun->GetAttackPower()))
				{
					m_pEvent = new cEvent;
					m_pEvent->Setup("PlayerUI/gameOver.png",
						"PlayerUI/gameOver.png");

					m_ePrevState = m_eState = GAME_OVER;
				}
			}
		}
	}
}

void cPlayScene::SettingBuildingGroup()
{
	m_pvBuildingGroup.resize(4);

	m_pvBuildingGroup[0] = new cBuildingGroup;
	m_pvBuildingGroup[0]->SetCenter(D3DXVECTOR3(-20, 0, 20));

	m_pvBuildingGroup[1] = new cBuildingGroup;
	m_pvBuildingGroup[1]->SetCenter(D3DXVECTOR3( 20, 0, 20));

	m_pvBuildingGroup[2] = new cBuildingGroup;
	m_pvBuildingGroup[2]->SetCenter(D3DXVECTOR3(-20, 0,-20));

	m_pvBuildingGroup[3] = new cBuildingGroup;
	m_pvBuildingGroup[3]->SetCenter(D3DXVECTOR3( 20, 0,-20));

	for (size_t buildingNum = 0; buildingNum < m_pTextMap->GetBuildings().size(); buildingNum++)
	{
		m_pvBuildingGroup[0]->AddBuilding(m_pTextMap->GetBuildings()[buildingNum]);
		m_pvBuildingGroup[1]->AddBuilding(m_pTextMap->GetBuildings()[buildingNum]);
		m_pvBuildingGroup[2]->AddBuilding(m_pTextMap->GetBuildings()[buildingNum]);
		m_pvBuildingGroup[3]->AddBuilding(m_pTextMap->GetBuildings()[buildingNum]);
	}
}

void cPlayScene::UpdatePlayerBuildingGroup()
{
	D3DXVECTOR3 vPos = m_pPlayer->GetPosition();

	if (vPos.x < 0)
		if (vPos.z > 0)
			m_pPlayer->SetBuildings(m_pvBuildingGroup[0]);
		else
			m_pPlayer->SetBuildings(m_pvBuildingGroup[2]);
	else
		if(vPos.z > 0)
			m_pPlayer->SetBuildings(m_pvBuildingGroup[1]);
		else
			m_pPlayer->SetBuildings(m_pvBuildingGroup[3]);
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

void cPlayScene::LevUpCheck()
{
	if (m_pPlayer->GetGun()->GetMaxLv() < m_pPlayer->GetGun()->GetCurrentLv())
		return;

	if (m_pPlayer->GetGun()->GetCurrentExp() >= m_pPlayer->GetGun()->GetMaxExp()) // 경험치 다 오르면
	{
		m_pPlayer->GetGun()->SetCurrentLv(m_pPlayer->GetGun()->GetCurrentLv() + 1); // 현재 레벨 증가
		m_pPlayer->GetGun()->SetCurrentExp(0); // 현재 경험치 초기화
		m_pPlayer->GetGun()->SetMaxExp(m_pPlayer->GetGun()->GetMaxExp() + 2); // 필요경험치 증가


		switch (m_pPlayer->GetGun()->GetCurrentLv())
		{
		case 1:
			m_pEvent = new cEvent;
			m_pEvent->Setup("PlayerUI/secondclassshooter.png",
				"PlayerUI/secondclassshooter.png");
			m_pPlayer->GetGun()->SetAttackPower(m_pPlayer->GetGun()->GetAttackPower() + 5);
			break;
		case 2:
			m_pEvent = new cEvent;
			m_pEvent->Setup("PlayerUI/firstclassshooter.png",
				"PlayerUI/firstclassshooter.png");
			//m_pPlayer->SetMoveSpeed(m_pPlayer->GetMoveSpeed() + 0.05f);
			break;
		case 3:
			m_pEvent = new cEvent;
			m_pEvent->Setup("PlayerUI/expressshooter.png",
				"PlayerUI/expressshooter.png");
			m_pPlayer->GetGun()->SetMagazine(m_pPlayer->GetGun()->GetMagazine() + 5);
			break;
		}
	}

}

void cPlayScene::QuitGameUISetting()
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	m_pQuitGameRoot = new cUIObject;
	m_pQuitGameRoot->SetPosition(0, 0);

	cUIImageView * QuitGameWindow = new cUIImageView;
	QuitGameWindow->SetTexture("QuitUI/QuitGameWindow.png");
	QuitGameWindow->SetPosition(CENTERX / 2, CENTERY / 2 - 50);

	cUIButton* YesButton = new cUIButton;
	YesButton->SetTexture("QuitUI/YesButton_1.png", "QuitUI/YesButton_2.png", "QuitUI/YesButton_3.png");
	YesButton->SetPosition(CENTERX / 2 - 260, CENTERY / 2 + 30);
	YesButton->SetTag(EXIT_BTN);
	YesButton->SetDelegate(this);

	cUIButton* NoButton = new cUIButton;
	NoButton->SetTexture("QuitUI/NoButton_1.png", "QuitUI/NoButton_2.png", "QuitUI/NoButton_3.png");
	NoButton->SetPosition(CENTERX / 2 - 260, CENTERY / 2 - 60);
	NoButton->SetTag(RETURN_BTN);
	NoButton->SetDelegate(this);

	m_pQuitGameRoot->AddChild(QuitGameWindow);
	m_pQuitGameRoot->AddChild(YesButton);
	m_pQuitGameRoot->AddChild(NoButton);
}

void cPlayScene::OnClick(cUIButton * pSender)
{
	switch (pSender->GetTag())
	{
	case EXIT_BTN:
		g_pSceneManager->ChangeScene("firstScene");
		break;
	case RETURN_BTN:
		m_eState = m_ePrevState;
		break;
	}
}
