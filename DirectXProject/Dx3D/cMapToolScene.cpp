#include "stdafx.h"
#include "cMapToolScene.h"
#include "cGrid.h"
#include "cMapCamera.h"
#include "cBuilding.h"
#include "cUIImageView.h"
#include "cUITextView.h"


cMapToolScene::cMapToolScene()
	: m_pBuilding(NULL)
	, m_pCamera(NULL)
	, m_pGrid(NULL)
{
}


cMapToolScene::~cMapToolScene()
{
	if (m_pUIRoot)
		m_pUIRoot->Destroy();

	for each (auto p in m_vpBuildings)
		SAFE_RELEASE(p)

	SAFE_RELEASE(m_pSprite);
//	SAFE_RELEASE(m_pBuilding);

	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);


}

void cMapToolScene::Setup()
{
	m_pBuilding = new cBuilding("Map/building/", "barrel.X");
	m_vpBuildings.push_back(m_pBuilding);

	cBuilding* a = new cBuilding("Map/building/", "barrel.X");

	a->SetPosition(D3DXVECTOR3(3, 0, 0));
	m_vpBuildings.push_back(a);

	m_pGrid = new cGrid;
	m_pGrid->Setup();

	m_pCamera = new cMapCamera;
	m_pCamera->Setup(NULL);

	SettingUI();
}

void cMapToolScene::Update()
{
	if (m_pCamera)
		m_pCamera->Update();

	if (m_pUIRoot)
		m_pUIRoot->Update();

	PositionSettingController();

	/*if (m_pBuilding)
		m_pBuilding->Update();*/
	for (size_t i = 0; i < m_vpBuildings.size(); i++)
	{
		m_vpBuildings[i]->Update();
	}
}

void cMapToolScene::Render()
{
	if (m_pGrid)
		m_pGrid->Render();

	if (m_pUIRoot)
		m_pUIRoot->Render(m_pSprite);

	//if (m_pBuilding)
	//	m_pBuilding->Render();

	for (size_t i = 0; i < m_vpBuildings.size(); i++)
	{
		m_vpBuildings[i]->Render();
	}

	char szTemp[1024];
	sprintf(szTemp, "¸ÊÅø");

	LPD3DXFONT pFont = g_pFontManager->GetFont(cFontManager::E_DEFAULT);
	RECT rc;
	SetRect(&rc, 0, 100, 300, 300);
	pFont->DrawText(NULL,
		szTemp,
		strlen(szTemp),
		&rc,
		DT_LEFT | DT_TOP | DT_WORDBREAK,
		D3DCOLOR_XRGB(255, 255, 0));

}

void cMapToolScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
	{
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
	}
}

void cMapToolScene::OnClick(cUIButton * pSender)
{
	switch (pSender->GetTag())
	{
	case PREV_TITLE:
		break;
	case NEXT_TITLE:
		break;
	case DOWN_SCALE:
		m_pBuilding->SetScale(m_pBuilding->GetScale() - 0.001f);
		break;
	case UP_SCALE:
		m_pBuilding->SetScale(m_pBuilding->GetScale() + 0.001f);
		break;
	case DOWN_ANGLE:
		m_pBuilding->SetAngle(m_pBuilding->GetAngle() - 0.03f);
		break;
	case UP_ANGLE:
		m_pBuilding->SetAngle(m_pBuilding->GetAngle() + 0.03f);
		break;
	case SAVE:
		SaveMapFile();
		break;
	default:
		break;
	}
}

void cMapToolScene::SettingUI()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	m_pUIRoot = new cUIObject;
	m_pUIRoot->SetPosition(0, 0);

	cUIImageView* pImageBackground = new cUIImageView;
	pImageBackground->SetTexture("Map/UI/background.png");
	pImageBackground->SetPosition(rc.right - pImageBackground->GetSize().nWidth / 2, pImageBackground->GetSize().nHeight / 2);

	/// >> : Title
	cUIButton* pLeftTitleBtn = new cUIButton;
	pLeftTitleBtn->SetTexture("Map/UI/arrow-btn-l-down.png", "Map/UI/arrow-btn-l-over.png", "Map/UI/arrow-btn-l-up.png");
	pLeftTitleBtn->SetPosition(pImageBackground->GetPosition().x - pImageBackground->GetSize().nWidth / 2, 30);
	pLeftTitleBtn->SetTag(PREV_TITLE);
	pLeftTitleBtn->SetDelegate(this);

	cUIButton* pRightTitleBtn = new cUIButton;
	pRightTitleBtn->SetTexture("Map/UI/arrow-btn-r-down.png", "Map/UI/arrow-btn-r-over.png", "Map/UI/arrow-btn-r-up.png");
	pRightTitleBtn->SetPosition(rc.right - pRightTitleBtn->GetSize().nWidth, 30);
	pRightTitleBtn->SetTag(NEXT_TITLE);
	pRightTitleBtn->SetDelegate(this);

	cUITextView* pTitleText = new cUITextView;
	pTitleText->SetText("title");
	pTitleText->SetSize(ST_SIZEN(pImageBackground->GetSize().nWidth - pRightTitleBtn->GetSize().nWidth * 2, 100));
	pTitleText->SetPosition(pLeftTitleBtn->GetPosition().x + pRightTitleBtn->GetSize().nWidth, pLeftTitleBtn->GetPosition().y);
	/// << :

	/// >> : SIze
	cUIButton* pScaleDownBtn = new cUIButton;
	pScaleDownBtn->SetTexture("Map/UI/arrow-btn-l-down.png", "Map/UI/arrow-btn-l-over.png", "Map/UI/arrow-btn-l-up.png");
	pScaleDownBtn->SetPosition(pImageBackground->GetPosition().x - pImageBackground->GetSize().nWidth / 2, 90);
	pScaleDownBtn->SetTag(DOWN_SCALE);
	pScaleDownBtn->SetDelegate(this);

	cUIButton* pScaleUpBtn = new cUIButton;
	pScaleUpBtn->SetTexture("Map/UI/arrow-btn-r-down.png", "Map/UI/arrow-btn-r-over.png", "Map/UI/arrow-btn-r-up.png");
	pScaleUpBtn->SetPosition(rc.right - pScaleUpBtn->GetSize().nWidth, 90);
	pScaleUpBtn->SetTag(UP_SCALE);
	pScaleUpBtn->SetDelegate(this);

	cUITextView* pScaleText = new cUITextView;
	pScaleText->SetText("scale");
	pScaleText->SetSize(ST_SIZEN(pImageBackground->GetSize().nWidth - pScaleUpBtn->GetSize().nWidth * 2, 100));
	pScaleText->SetPosition(pScaleDownBtn->GetPosition().x + pScaleUpBtn->GetSize().nWidth, pScaleDownBtn->GetPosition().y);
	/// << :

	/// >> : Angle
	cUIButton* pAngleDownBtn = new cUIButton;
	pAngleDownBtn->SetTexture("Map/UI/arrow-btn-l-down.png", "Map/UI/arrow-btn-l-over.png", "Map/UI/arrow-btn-l-up.png");
	pAngleDownBtn->SetPosition(pImageBackground->GetPosition().x - pImageBackground->GetSize().nWidth / 2, 150);
	pAngleDownBtn->SetTag(DOWN_ANGLE);
	pAngleDownBtn->SetDelegate(this);

	cUIButton* pAngleUpBtn = new cUIButton;
	pAngleUpBtn->SetTexture("Map/UI/arrow-btn-r-down.png", "Map/UI/arrow-btn-r-over.png", "Map/UI/arrow-btn-r-up.png");
	pAngleUpBtn->SetPosition(rc.right - pAngleUpBtn->GetSize().nWidth, 150);
	pAngleUpBtn->SetTag(UP_ANGLE);
	pAngleUpBtn->SetDelegate(this);

	cUITextView* pAngleText = new cUITextView;
	pAngleText->SetText("angle");
	pAngleText->SetSize(ST_SIZEN(pImageBackground->GetSize().nWidth - pAngleUpBtn->GetSize().nWidth * 2, 100));
	pAngleText->SetPosition(pAngleDownBtn->GetPosition().x + pAngleUpBtn->GetSize().nWidth, pAngleDownBtn->GetPosition().y);
	/// << :
	
	/// >> : Save
	cUIButton* pSaveBtn = new cUIButton;
	pSaveBtn->SetTexture("Map/UI/btn-med-down.png", "Map/UI/btn-med-over.png", "Map/UI/btn-med-up.png");
	pSaveBtn->SetPosition(pImageBackground->GetPosition().x - pImageBackground->GetSize().nWidth / 2 + 25, rc.bottom - pSaveBtn->GetSize().nHeight * 2 - 15);
	pSaveBtn->SetTag(SAVE);
	pSaveBtn->SetDelegate(this);

	cUITextView* pSaveText = new cUITextView;
	pSaveText->SetText("Save");
	pSaveText->SetSize(ST_SIZEN(pImageBackground->GetSize().nWidth - 50, 100));
	pSaveText->SetPosition(pSaveBtn->GetPosition().x, pSaveBtn->GetPosition().y);
	/// << :

	/// >> : Exit
	cUIButton* pExitBtn = new cUIButton;
	pExitBtn->SetTexture("Map/UI/btn-med-down.png", "Map/UI/btn-med-over.png", "Map/UI/btn-med-up.png");
	pExitBtn->SetPosition(pImageBackground->GetPosition().x - pImageBackground->GetSize().nWidth / 2 + 25, rc.bottom - pSaveBtn->GetSize().nHeight - 15);
	pExitBtn->SetTag(EXIT);
	pExitBtn->SetDelegate(this);

	cUITextView* pExitText = new cUITextView;
	pExitText->SetText("Exit");
	pExitText->SetSize(ST_SIZEN(pImageBackground->GetSize().nWidth - 50, 100));
	pExitText->SetPosition(pExitBtn->GetPosition().x, pExitBtn->GetPosition().y);
	/// << :

	m_pUIRoot->AddChild(pImageBackground);
	m_pUIRoot->AddChild(pLeftTitleBtn);
	m_pUIRoot->AddChild(pRightTitleBtn);
	m_pUIRoot->AddChild(pTitleText);
	m_pUIRoot->AddChild(pScaleDownBtn);
	m_pUIRoot->AddChild(pScaleUpBtn);
	m_pUIRoot->AddChild(pScaleText);
	m_pUIRoot->AddChild(pAngleDownBtn);
	m_pUIRoot->AddChild(pAngleUpBtn);
	m_pUIRoot->AddChild(pAngleText);
	m_pUIRoot->AddChild(pSaveBtn);
	m_pUIRoot->AddChild(pSaveText);
	m_pUIRoot->AddChild(pExitBtn);
	m_pUIRoot->AddChild(pExitText);
}

void cMapToolScene::PositionSettingController()
{
	D3DXVECTOR3 pos = m_pBuilding->GetPosition();

	if (g_pKeyManager->IsStayKeyDown(VK_LEFT))
		pos.x -= 0.1f;
	else if (g_pKeyManager->IsStayKeyDown(VK_RIGHT))
		pos.x += 0.1f;

	if (g_pKeyManager->IsStayKeyDown(VK_UP))
		pos.z += 0.1f;
	else if (g_pKeyManager->IsStayKeyDown(VK_DOWN))
		pos.z -= 0.1f;

	m_pBuilding->SetPosition(pos);

}

void cMapToolScene::SaveMapFile()
{
	HANDLE file;

	char str[MAPMAXSIZE] = { 0 };
	DWORD write;

	file = CreateFile("mapFile.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	for (size_t i = 0; i < m_vpBuildings.size(); i++)
	{
		strcat_s(str, "{\nDirectoryName : ");
		strcat_s(str, m_vpBuildings[i]->GetFolderName().c_str());	
		strcat_s(str, "\nFileName : ");
		strcat_s(str, m_vpBuildings[i]->GetFileName().c_str());
		strcat_s(str, "\nS : ");
		strcat_s(str, std::to_string(m_vpBuildings[i]->GetScale()).c_str());
		strcat_s(str, "\nA : ");
		strcat_s(str, std::to_string(m_vpBuildings[i]->GetAngle()).c_str());
		strcat_s(str, "\nP : ");
		strcat_s(str, std::to_string(m_vpBuildings[i]->GetPosition().x).c_str());
		strcat_s(str, ", ");
		strcat_s(str, std::to_string(m_vpBuildings[i]->GetPosition().y).c_str());
		strcat_s(str, ", ");
		strcat_s(str, std::to_string(m_vpBuildings[i]->GetPosition().z).c_str());
		strcat_s(str, "\n}\n");

	}

	WriteFile(file, str, 2048, &write, NULL);
	CloseHandle(file);
}
