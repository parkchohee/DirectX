#include "stdafx.h"
#include "cStaticMesh.h"
#include "cCamera.h"

LPD3DXEFFECT cStaticMesh::pMeshEffect = NULL;

cStaticMesh::cStaticMesh()
	: m_pStaticMesh(NULL)
	, m_vMtrls(0)
	, m_vTexture(0)
	, m_vMin(0, 0, 0)
	, m_vMax(0, 0, 0)
	, m_vPosition(0,0,0)
{
	D3DXMatrixIdentity(&m_pmatWorld);

	if (pMeshEffect == NULL)
		pMeshEffect = LoadEffect("Shader/XMeshStatic.fx");
}

cStaticMesh::cStaticMesh(char* szDirectory, char* szFilename)
	: m_pStaticMesh(NULL)
	, m_vMtrls(0)
	, m_vTexture(0)
	, m_vMin(0, 0, 0)
	, m_vMax(0, 0, 0)
{
	cStaticMesh* pStaticMesh = g_pStaticMeshManager->GetStaticMesh(szDirectory, szFilename);

	D3DXMatrixIdentity(&m_pmatWorld);

	m_pStaticMesh = pStaticMesh->m_pStaticMesh;
	m_vMtrls = pStaticMesh->m_vMtrls;
	m_vTexture = pStaticMesh->m_vTexture;

	m_vMin = pStaticMesh->m_vMin;
	m_vMax = pStaticMesh->m_vMax;
}

cStaticMesh::~cStaticMesh()
{
}

bool cStaticMesh::Setup(char* szDirectory, char* szFilename)
{
	HRESULT hr = 0;

	ID3DXBuffer* adjBuffer = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD numMtrls = 0;

	std::string sFullPath(szDirectory);
	sFullPath += std::string(szFilename);

	hr = D3DXLoadMeshFromX(
		sFullPath.c_str(),
		D3DXMESH_MANAGED,
		g_pD3DDevice,
		&adjBuffer,
		&mtrlBuffer,
		0,
		&numMtrls,
		&m_pStaticMesh);

	if (FAILED(hr))
	{
		::MessageBox(0, "D3DLoadMeshFromX() - FAILED", 0, 0);
		return false;
	}

	if (mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for (int i = 0; i < numMtrls; i++)
		{
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;
			m_vMtrls.push_back(mtrls[i].MatD3D);

			if (mtrls[i].pTextureFilename != 0)
			{
				IDirect3DTexture9* tex = 0;

				std::string sTexturePath(szDirectory);
				sTexturePath += std::string(mtrls[i].pTextureFilename);

				D3DXCreateTextureFromFile(g_pD3DDevice,
					sTexturePath.c_str(),
					&tex);

				m_vTexture.push_back(tex);
			}
			else
			{
				m_vTexture.push_back(0);
			}
		}
	}

	// 메시에서 vertex를 가져온다. 
	LPDIRECT3DVERTEXBUFFER9 vb = 0;
	m_pStaticMesh->GetVertexBuffer(&vb);
	ST_PNT_VERTEX * vertices = new ST_PNT_VERTEX[m_pStaticMesh->GetNumVertices()];
	
	VOID* pVertices;
	vb->Lock(0, sizeof(vertices), (void**)&pVertices, 0);
	memcpy(vertices, pVertices, sizeof(ST_PNT_VERTEX)*m_pStaticMesh->GetNumVertices());
	vb->Unlock();

	SAFE_RELEASE(vb);

	// vertex중에서 max와 min을 셋팅..
	for (int i = 0; i < m_pStaticMesh->GetNumVertices(); i++)
	{
		D3DXVec3Minimize(&m_vMin, &m_vMin, &vertices[i].p);
		D3DXVec3Maximize(&m_vMax, &m_vMax, &vertices[i].p);
	}

	return true;
}

void cStaticMesh::Render()
{
	if (pMeshEffect == NULL)
		pMeshEffect = LoadEffect("Shader/XMeshStatic.fx");
	
	pMeshEffect->SetTechnique("Base");
	pMeshEffect->SetMatrix("matWorld", &m_pmatWorld);
	
	D3DXMATRIXA16 matProj, matView;
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);

	matProj = matView * matProj;
	pMeshEffect->SetMatrix("matViewProjection", &matProj);

	UINT pass;
	pMeshEffect->Begin(&pass, 0);

	for (UINT p = 0; p < pass; p++) {

		pMeshEffect->BeginPass(p);

		//재질수는 메쉬의 Subset 수와 동일하다.
		//따라서 각 서브셋을 랜더링하기 위해 메쉬 수만큼 랜더링을 하면 된다.
		for (DWORD i = 0; i < m_vMtrls.size(); i++)
		{
			pMeshEffect->SetTexture("Normal_Tex", m_vTexture[i]);
			pMeshEffect->SetTexture("Diffuse_Tex", m_vTexture[i]);
			pMeshEffect->SetTexture("Specular_Tex", m_vTexture[i]);
			pMeshEffect->SetTexture("Emission_Tex", m_vTexture[i]);
			pMeshEffect->SetTexture("Reflect_Tex", m_vTexture[i]);

			//일반적으로 Effect 의 Begin 이후 부터 셰이더의 변수값(프로퍼티)을
			//변경할 수 없지만.

			//Begin 이 후에 변경하기 위해서는 다음과 같은 코드를 써주면된다.
			pMeshEffect->CommitChanges();


			//해당 서브셋 드로우
			m_pStaticMesh->DrawSubset(i);
		}

		pMeshEffect->EndPass();

	}

	pMeshEffect->End();


	/*g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_pmatWorld);

	for (int i = 0; i < m_vMtrls.size(); i++)
	{
		g_pD3DDevice->SetMaterial(&m_vMtrls[i]);
		g_pD3DDevice->SetTexture(0, m_vTexture[i]);
		m_pStaticMesh->DrawSubset(i);
	}*/
}

void cStaticMesh::RenderShadow(cCamera * camera)
{
	if (pMeshEffect == NULL)
		pMeshEffect = LoadEffect("Shader/XMeshStatic.fx");

	pMeshEffect->SetTechnique("Shadow");

	//월드 
	pMeshEffect->SetMatrix("matWorld", &m_pmatWorld);

	//뷰
	pMeshEffect->SetMatrix("matViewProjection", &camera->matViewProjection);

	UINT pass;
	pMeshEffect->Begin(&pass, 0);

	for (UINT p = 0; p < pass; p++) {

		pMeshEffect->BeginPass(p);

		//재질수는 메쉬의 Subset 수와 동일하다.
		//따라서 각 서브셋을 랜더링하기 위해 메쉬 수만큼 랜더링을 하면 된다.
		for (DWORD i = 0; i < m_vMtrls.size(); i++)
		{
			//해당 서브셋 드로우
			m_pStaticMesh->DrawSubset(i);
		}

		pMeshEffect->EndPass();
	}

	pMeshEffect->End();
}

void cStaticMesh::Destroy()
{
	for each (auto p in m_vTexture)
		SAFE_RELEASE(p);

	SAFE_RELEASE(m_pStaticMesh);
	SAFE_RELEASE(pMeshEffect);
}

LPD3DXEFFECT cStaticMesh::LoadEffect(char * szFilename)
{
	LPD3DXEFFECT pEffect;

	//에러 내용을 받아올 에러 버퍼
	LPD3DXBUFFER pError = NULL;

	//셰이더를 로딩과 동시에 컴파일 한다.
	D3DXCreateEffectFromFile(
		g_pD3DDevice,				//디바이스를 넘긴다.
		szFilename,	//로딩될 셰이더 코드 경로 이름
		NULL,				//매크로 설정 안씀 NULL
		NULL,				//포함설정 안씀 NULL, 
		0,					//플레그 0
		NULL,				//로딩 메모리 풀 그냥  NULL
		&pEffect,			//로딩된 Effect 가 저장될 LPD3DXEFFECT 더블포인터
		&pError				//Error 버퍼 ( 만약 LPD3DXBUFFER 의 주소가 대입되면 컴파일과정중 에러가 나면 여기 에러메시지가 들어간다 제대로 컴파일되면 NULL 이 된다 )
	);

	//셰이더 코드 컴파일 로딩중 문제가 있다...
	if (pError != NULL && pEffect == NULL)
	{
		char* temp = (char*)pError->GetBufferPointer();
		//사용된 버퍼는 해재하자.
		SAFE_RELEASE(pError);

		return NULL;
	}


	return pEffect;
}
