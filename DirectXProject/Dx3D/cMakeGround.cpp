#include "stdafx.h"
#include "cMakeGround.h"
#include "cRay.h"
#include "cUIImageView.h"
#include "cUITextView.h"


cMakeGround::cMakeGround()
	: m_pTexture(NULL)
	, m_pMesh(NULL)
{
}


cMakeGround::~cMakeGround()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pSprite);

	if (m_pUIRoot)
		m_pUIRoot->Destroy();
}

void cMakeGround::Setup()
{
	m_pTexture = g_pTextureManager->GetTexture("Map/Ground_CMGround_CM.tga");

	m_vecGround.resize(6);
	m_vecGround[0] = D3DXVECTOR3(-MAPSIZE / 2, 0, MAPSIZE / 2);
	m_vecGround[1] = D3DXVECTOR3(MAPSIZE / 2, 0, -MAPSIZE / 2);
	m_vecGround[2] = D3DXVECTOR3(-MAPSIZE / 2, 0, -MAPSIZE / 2);
	m_vecGround[3] = D3DXVECTOR3(-MAPSIZE / 2, 0, MAPSIZE / 2);
	m_vecGround[4] = D3DXVECTOR3(MAPSIZE / 2, 0, MAPSIZE / 2);
	m_vecGround[5] = D3DXVECTOR3(MAPSIZE / 2, 0, -MAPSIZE / 2);

	m_nTileN = MAPSIZE - 1;
	int nNumVertex = MAPSIZE * MAPSIZE;

	//std::vector<ST_PNT_VERTEX>	vecVertex(nNumVertex);
	m_vecVertex.resize(nNumVertex);

	/*std::vector<DWORD>			vecIndex;*/
	m_vecIndex.reserve(m_nTileN * m_nTileN * 2 * 3);

	for (int i = 0; i < nNumVertex; ++i)
	{
		ST_PNT_VERTEX v;
		v.p = D3DXVECTOR3(i % MAPSIZE - (MAPSIZE / 2), 0, i / MAPSIZE - (MAPSIZE / 2));
		v.n = D3DXVECTOR3(0, 1, 0);
		v.t = D3DXVECTOR2((i % MAPSIZE) / (float)MAPSIZE * 5, (i / MAPSIZE) / (float)MAPSIZE * 5);
		m_vecVertex[i] = v;
		//m_vecVertex[i] = v.p;
	}

	SetNormal();

	for (int x = 0; x < m_nTileN; ++x)
	{
		for (int z = 0; z < m_nTileN; ++z)
		{
			int _0 = (z + 0) * MAPSIZE + x + 0;
			int _1 = (z + 1) * MAPSIZE + x + 0;
			int _2 = (z + 0) * MAPSIZE + x + 1;
			int _3 = (z + 1) * MAPSIZE + x + 1;

			m_vecIndex.push_back(_0); m_vecIndex.push_back(_1); m_vecIndex.push_back(_2);
			m_vecIndex.push_back(_3); m_vecIndex.push_back(_2); m_vecIndex.push_back(_1);
		}
	}

	///>> : 
	D3DXCreateMeshFVF(
		m_vecIndex.size() / 3,
		m_vecVertex.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		ST_PNT_VERTEX::FVF,
		g_pD3DDevice,
		&m_pMesh);

	ST_PNT_VERTEX* pV = NULL;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &m_vecVertex[0], m_vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	DWORD* pI = NULL;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	memcpy(pI, &m_vecIndex[0], m_vecIndex.size() * sizeof(DWORD));
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	m_pMesh->LockAttributeBuffer(0, &pA);
	ZeroMemory(pA, (m_vecIndex.size() / 3) * sizeof(DWORD));
	m_pMesh->UnlockAttributeBuffer();

	std::vector<DWORD> vecAdj(m_vecIndex.size());
	m_pMesh->GenerateAdjacency(0.0f, &vecAdj[0]);

	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdj[0],
		0, 0, 0);

	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	m_stMtl.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	m_stMtl.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);

	SettingUI();
}

void cMakeGround::Update(POINT mouse)
{
	if (m_pUIRoot)
		m_pUIRoot->Update();
	
	// 높이를 조절해준다....

	// pick 해서
	if (g_pKeyManager->IsStayKeyDown(VK_LBUTTON))
	{
		D3DXVECTOR3 PickedPosition;
		cRay r = cRay::RayAtWorldSpace(mouse.x, mouse.y);
		bool isPicked = false;

		for (size_t i = 0; i < m_vecGround.size(); i += 3)
		{
			D3DXVECTOR3 Position;
			if (!r.IntersectTri(m_vecGround[i + 0], m_vecGround[i + 1], m_vecGround[i + 2], Position))
				continue;
			
			isPicked = true;
			PickedPosition = Position;
		}

		if (isPicked)
		{
			// x z 값 구하고
			int nX = PickedPosition.x + MAPSIZE / 2-1;
			int nZ = PickedPosition.z + MAPSIZE / 2-1;

			// 삼각형 위치의 인덱스를 구해서 
			int _0 = (nZ + 0) * (m_nTileN + 1) + nX + 0;
			int _1 = (nZ + 1) * (m_nTileN + 1) + nX + 0;
			int _2 = (nZ + 0) * (m_nTileN + 1) + nX + 1;
			int _3 = (nZ + 1) * (m_nTileN + 1) + nX + 1;
			
			// y값 증가시킨다. 
			m_vecVertex[_0].p.y += 0.1f;
			m_vecVertex[_1].p.y += 0.1f;
			m_vecVertex[_2].p.y += 0.1f;
			m_vecVertex[_3].p.y += 0.1f;

			ST_PNT_VERTEX* pV = NULL;
			m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
			memcpy(pV, &m_vecVertex[0], m_vecVertex.size() * sizeof(ST_PNT_VERTEX));
			m_pMesh->UnlockVertexBuffer();

			DWORD* pI = NULL;
			m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
			memcpy(pI, &m_vecIndex[0], m_vecIndex.size() * sizeof(DWORD));
			m_pMesh->UnlockIndexBuffer();
		}

		SetNormal();
	}

	//if (g_pKeyManager->IsOnceKeyDown(VK_SPACE))
	//	g_pSceneManager->ChangeScene("playScene");

}

void cMakeGround::Render()
{
	//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetTexture(0, m_pTexture);
	m_pMesh->DrawSubset(0);

	if (m_pUIRoot)
		m_pUIRoot->Render(m_pSprite);

}

void cMakeGround::SetNormal()
{
	for (int x = 1; x < m_nTileN; ++x)
	{
		for (int z = 1; z < m_nTileN; ++z)
		{
			int left = (z + 0) * MAPSIZE + x - 1;
			int right = (z + 0) * MAPSIZE + x + 1;
			int up = (z + 1) * MAPSIZE + x + 0;
			int down = (z - 1) * MAPSIZE + x + 0;

			D3DXVECTOR3 leftToRight = m_vecVertex[right].p - m_vecVertex[left].p;
			D3DXVECTOR3 downToUp = m_vecVertex[up].p - m_vecVertex[down].p;
			D3DXVECTOR3 normal;
			D3DXVec3Cross(&normal, &downToUp, &leftToRight);
			D3DXVec3Normalize(&normal, &normal);

			int nIndex = z * MAPSIZE + x;
			m_vecVertex[nIndex].n = normal;
		}
	}
}

void cMakeGround::SettingUI()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	m_pUIRoot = new cUIObject;
	m_pUIRoot->SetPosition(0, 0);

	cUITextView* pTitleText = new cUITextView;
	pTitleText->SetText("지형 편집");
	pTitleText->SetSize(ST_SIZEN(200, 100));
	pTitleText->SetPosition(rc.right - 250, rc.top + 30);

	m_pUIRoot->AddChild(pTitleText);

}

void cMakeGround::SaveMapFile()
{
	HANDLE file;

	char str[50000] = { 0 };
	DWORD write;

	file = CreateFile("heightMap.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	for (size_t i = 0; i < m_vecVertex.size(); i++)
	{
		strcat_s(str, std::to_string((int)((float)m_vecVertex[i].p.y * 10)).c_str());
		strcat_s(str, " ");
	}

	WriteFile(file, str, 50000, &write, NULL);
	CloseHandle(file);
}
