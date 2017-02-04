#include "stdafx.h"
#include "cMakeGround.h"
#include "cRay.h"


cMakeGround::cMakeGround()
	: m_pTexture(NULL)
	, m_pMesh(NULL)
{
}


cMakeGround::~cMakeGround()
{
	SAFE_RELEASE(m_pMesh);
}

void cMakeGround::Setup()
{
	m_vecGround.resize(6);
	m_vecGround[0] = D3DXVECTOR3(-MAPSIZE / 2, 0, MAPSIZE / 2);
	m_vecGround[1] = D3DXVECTOR3(MAPSIZE / 2, 0, -MAPSIZE / 2);
	m_vecGround[2] = D3DXVECTOR3(-MAPSIZE / 2, 0, -MAPSIZE / 2);
	m_vecGround[3] = D3DXVECTOR3(-MAPSIZE / 2, 0, MAPSIZE / 2);
	m_vecGround[4] = D3DXVECTOR3(MAPSIZE / 2, 0, MAPSIZE / 2);
	m_vecGround[5] = D3DXVECTOR3(MAPSIZE / 2, 0, -MAPSIZE / 2);

	m_nTileN = MAPSIZE - 1;
	int nNumVertex = MAPSIZE * MAPSIZE;

	std::vector<ST_PNT_VERTEX>	vecVertex(nNumVertex);
	m_vecVertex.resize(nNumVertex);

	std::vector<DWORD>			vecIndex;
	vecIndex.reserve(m_nTileN * m_nTileN * 2 * 3);

	for (int i = 0; i < nNumVertex; ++i)
	{
		ST_PNT_VERTEX v;
		v.p = D3DXVECTOR3(i % MAPSIZE - (MAPSIZE / 2), 0, i / MAPSIZE - (MAPSIZE / 2));
		v.n = D3DXVECTOR3(0, 1, 0);
		v.t = D3DXVECTOR2((i % MAPSIZE) / (float)MAPSIZE, (i / MAPSIZE) / (float)MAPSIZE);
		vecVertex[i] = v;
		m_vecVertex[i] = v.p;
	}

	for (int x = 1; x < m_nTileN; ++x)
	{
		for (int z = 1; z < m_nTileN; ++z)
		{
			int left = (z + 0) * MAPSIZE + x - 1;
			int right = (z + 0) * MAPSIZE + x + 1;
			int up = (z + 1) * MAPSIZE + x + 0;
			int down = (z - 1) * MAPSIZE + x + 0;

			D3DXVECTOR3 leftToRight = m_vecVertex[right] - m_vecVertex[left];
			D3DXVECTOR3 downToUp = m_vecVertex[up] - m_vecVertex[down];
			D3DXVECTOR3 normal;
			D3DXVec3Cross(&normal, &downToUp, &leftToRight);
			D3DXVec3Normalize(&normal, &normal);

			int nIndex = z * MAPSIZE + x;
			vecVertex[nIndex].n = normal;
		}
	}

	for (int x = 0; x < m_nTileN; ++x)
	{
		for (int z = 0; z < m_nTileN; ++z)
		{
			int _0 = (z + 0) * MAPSIZE + x + 0;
			int _1 = (z + 1) * MAPSIZE + x + 0;
			int _2 = (z + 0) * MAPSIZE + x + 1;
			int _3 = (z + 1) * MAPSIZE + x + 1;

			vecIndex.push_back(_0); vecIndex.push_back(_1); vecIndex.push_back(_2);
			vecIndex.push_back(_3); vecIndex.push_back(_2); vecIndex.push_back(_1);
		}
	}

	///>> : 
	D3DXCreateMeshFVF(
		vecIndex.size() / 3,
		vecVertex.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		ST_PNT_VERTEX::FVF,
		g_pD3DDevice,
		&m_pMesh);

	ST_PNT_VERTEX* pV = NULL;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	DWORD* pI = NULL;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	m_pMesh->LockAttributeBuffer(0, &pA);
	ZeroMemory(pA, (vecIndex.size() / 3) * sizeof(DWORD));
	m_pMesh->UnlockAttributeBuffer();

	std::vector<DWORD> vecAdj(vecIndex.size());
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

}

void cMakeGround::Update(POINT mouse)
{
	// ���̸� �������ش�....

	// pick �ؼ�
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
			// x z �� ���ϰ�
			
			int nX = PickedPosition.x + MAPSIZE / 2;
			int nZ = PickedPosition.z + MAPSIZE / 2;

			float fDeltaX = PickedPosition.x - nX;
			float fDeltaZ = PickedPosition.z - nZ;

			// �ﰢ�� ��ġ�� �ε����� ���ؼ� 
			int _0 = (nZ + 0) * (m_nTileN + 1) + nX + 0;
			int _1 = (nZ + 1) * (m_nTileN + 1) + nX + 0;
			int _2 = (nZ + 0) * (m_nTileN + 1) + nX + 1;
			int _3 = (nZ + 1) * (m_nTileN + 1) + nX + 1;
			
			// y�� ������Ų��. 
			m_vecVertex[_0].y++;
			m_vecVertex[_1].y++;
			m_vecVertex[_2].y++;
			m_vecVertex[_3].y++;

			int a = 0;
		}

	}
	

	if (g_pKeyManager->IsOnceKeyDown('L'))
		SaveMapFile();


}

void cMakeGround::Render()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetTexture(0, m_pTexture);
	m_pMesh->DrawSubset(0);
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
		strcat_s(str, std::to_string((int)m_vecVertex[i].y).c_str());
		strcat_s(str, " ");
	}

	WriteFile(file, str, 50000, &write, NULL);
	CloseHandle(file);
}