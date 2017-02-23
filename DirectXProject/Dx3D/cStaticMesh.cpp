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

	// �޽ÿ��� vertex�� �����´�. 
	LPDIRECT3DVERTEXBUFFER9 vb = 0;
	m_pStaticMesh->GetVertexBuffer(&vb);
	ST_PNT_VERTEX * vertices = new ST_PNT_VERTEX[m_pStaticMesh->GetNumVertices()];
	
	VOID* pVertices;
	vb->Lock(0, sizeof(vertices), (void**)&pVertices, 0);
	memcpy(vertices, pVertices, sizeof(ST_PNT_VERTEX)*m_pStaticMesh->GetNumVertices());
	vb->Unlock();

	SAFE_RELEASE(vb);

	// vertex�߿��� max�� min�� ����..
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

		//�������� �޽��� Subset ���� �����ϴ�.
		//���� �� ������� �������ϱ� ���� �޽� ����ŭ �������� �ϸ� �ȴ�.
		for (DWORD i = 0; i < m_vMtrls.size(); i++)
		{
			pMeshEffect->SetTexture("Normal_Tex", m_vTexture[i]);
			pMeshEffect->SetTexture("Diffuse_Tex", m_vTexture[i]);
			pMeshEffect->SetTexture("Specular_Tex", m_vTexture[i]);
			pMeshEffect->SetTexture("Emission_Tex", m_vTexture[i]);
			pMeshEffect->SetTexture("Reflect_Tex", m_vTexture[i]);

			//�Ϲ������� Effect �� Begin ���� ���� ���̴��� ������(������Ƽ)��
			//������ �� ������.

			//Begin �� �Ŀ� �����ϱ� ���ؼ��� ������ ���� �ڵ带 ���ָ�ȴ�.
			pMeshEffect->CommitChanges();


			//�ش� ����� ��ο�
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

	//���� 
	pMeshEffect->SetMatrix("matWorld", &m_pmatWorld);

	//��
	pMeshEffect->SetMatrix("matViewProjection", &camera->matViewProjection);

	UINT pass;
	pMeshEffect->Begin(&pass, 0);

	for (UINT p = 0; p < pass; p++) {

		pMeshEffect->BeginPass(p);

		//�������� �޽��� Subset ���� �����ϴ�.
		//���� �� ������� �������ϱ� ���� �޽� ����ŭ �������� �ϸ� �ȴ�.
		for (DWORD i = 0; i < m_vMtrls.size(); i++)
		{
			//�ش� ����� ��ο�
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

	//���� ������ �޾ƿ� ���� ����
	LPD3DXBUFFER pError = NULL;

	//���̴��� �ε��� ���ÿ� ������ �Ѵ�.
	D3DXCreateEffectFromFile(
		g_pD3DDevice,				//����̽��� �ѱ��.
		szFilename,	//�ε��� ���̴� �ڵ� ��� �̸�
		NULL,				//��ũ�� ���� �Ⱦ� NULL
		NULL,				//���Լ��� �Ⱦ� NULL, 
		0,					//�÷��� 0
		NULL,				//�ε� �޸� Ǯ �׳�  NULL
		&pEffect,			//�ε��� Effect �� ����� LPD3DXEFFECT ����������
		&pError				//Error ���� ( ���� LPD3DXBUFFER �� �ּҰ� ���ԵǸ� �����ϰ����� ������ ���� ���� �����޽����� ���� ����� �����ϵǸ� NULL �� �ȴ� )
	);

	//���̴� �ڵ� ������ �ε��� ������ �ִ�...
	if (pError != NULL && pEffect == NULL)
	{
		char* temp = (char*)pError->GetBufferPointer();
		//���� ���۴� ��������.
		SAFE_RELEASE(pError);

		return NULL;
	}


	return pEffect;
}
