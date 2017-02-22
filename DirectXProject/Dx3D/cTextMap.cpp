#include "stdafx.h"
#include "cTextMap.h"
#include "cBuilding.h"
#include "cFrustum.h"
#include "cOBB.h"


cTextMap::cTextMap()
	: m_pFrustum(NULL)
{
}


cTextMap::~cTextMap()
{
	SAFE_DELETE(m_pFrustum);

	for each (auto p in m_vpBuildings)
		SAFE_RELEASE(p);

	m_vpBuildings.clear();
}

void cTextMap::Setup(IN char * szFilename, IN D3DXMATRIXA16 * pmat)
{
	m_pFrustum = new cFrustum;
	m_pFrustum->Setup();

	FILE* fp = NULL;
	fopen_s(&fp, szFilename, "r");

	cBuilding* pBuiling;
	char szTempFolderName[128];
	char szTempFileName[128];

	while (true)
	{
		if (feof(fp))
			break;

		char szTemp[1024];
		fgets(szTemp, 1024, fp);

		if (szTemp[0] == '{')
		{
			continue;
		}
		else if (szTemp[0] == 'D')
		{
			sscanf_s(szTemp, "%*s : %s", szTempFolderName, 128);
		}
		else if (szTemp[0] == 'F')
		{
			sscanf_s(szTemp, "%*s : %s", szTempFileName, 128);
			pBuiling = new cBuilding(szTempFolderName, szTempFileName);
			pBuiling->SetOBB(szTempFolderName, szTempFileName);
		}
		else if (szTemp[0] == 'S')
		{
			float scale = 0.f;
			sscanf_s(szTemp, "%*s : %f", &scale);

			pBuiling->SetScale(scale);
		}
		else if (szTemp[0] == 'A')
		{
			float angle = 0.f;
			sscanf_s(szTemp, "%*s : %f", &angle);

			pBuiling->SetAngle(angle);
		}
		else if (szTemp[0] == 'P')
		{
			D3DXVECTOR3 position;
			sscanf_s(szTemp, "%*s : %f, %f, %f", &position.x, &position.y, &position.z);

			pBuiling->SetPosition(position);
		}
		else if (szTemp[0] == '}')
		{
			m_vpBuildings.push_back(pBuiling);
		}
	}

	fclose(fp);
}

void cTextMap::Update()
{
	m_pFrustum->Update();

	for (size_t i = 0; i < m_vpBuildings.size(); i++)
	{
		m_vpBuildings[i]->Update();
	}
}

void cTextMap::Render()
{
	for (size_t i = 0; i < m_vpBuildings.size(); i++)
	{
		if (m_pFrustum->IsIn(m_vpBuildings[i]->GetPosition(), m_vpBuildings[i]->GetOBB()->GetRadius()))
			m_vpBuildings[i]->Render();
	}
}

bool cTextMap::GetHeight(IN float x, OUT float & y, IN float z)
{
	return true;
}
