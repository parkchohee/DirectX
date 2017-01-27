#include "StdAfx.h"
#include "cOBB.h"
#include "cSkinnedMesh.h"
#include "cStaticMesh.h"

cOBB::cOBB(void)
{
}


cOBB::~cOBB(void)
{
}

/// : �Լ� ���ڷ� �Ѱ��ִ� ���� ���ڰ� ���� ��� �Ѵ� ����� ����
void cOBB::Setup( cSkinnedMesh* pSkinnedMesh )
{
	D3DXVECTOR3 vMin = pSkinnedMesh->GetMin();
	D3DXVECTOR3 vMax = pSkinnedMesh->GetMax();
	m_vOrgCenterPos = (vMin + vMax) / 2.f;

	m_vOrgAxisDir[0] = D3DXVECTOR3(1, 0, 0);
	m_vOrgAxisDir[1] = D3DXVECTOR3(0, 1, 0);
	m_vOrgAxisDir[2] = D3DXVECTOR3(0, 0, 1);

	m_fAxisLen[0] = fabs(vMax.x - vMin.x);
	m_fAxisLen[1] = fabs(vMax.y - vMin.y);
	m_fAxisLen[2] = fabs(vMax.z - vMin.z);

	m_fAxisHalfLen[0] = m_fAxisLen[0] / 2.0f;
	m_fAxisHalfLen[1] = m_fAxisLen[1] / 2.0f;
	m_fAxisHalfLen[2] = m_fAxisLen[2] / 2.0f;

	D3DXMatrixIdentity(&m_matWorldTM);
}

void cOBB::Setup(cStaticMesh * pStaticMesh)
{
	D3DXVECTOR3 vMin = pStaticMesh->GetMin();
	D3DXVECTOR3 vMax = pStaticMesh->GetMax();
	m_vOrgCenterPos = (vMin + vMax) / 2.f;

	m_vOrgAxisDir[0] = D3DXVECTOR3(1, 0, 0);
	m_vOrgAxisDir[1] = D3DXVECTOR3(0, 1, 0);
	m_vOrgAxisDir[2] = D3DXVECTOR3(0, 0, 1);

	m_fAxisLen[0] = fabs(vMax.x - vMin.x);
	m_fAxisLen[1] = fabs(vMax.y - vMin.y);
	m_fAxisLen[2] = fabs(vMax.z - vMin.z);

	m_fAxisHalfLen[0] = m_fAxisLen[0] / 2.0f;
	m_fAxisHalfLen[1] = m_fAxisLen[1] / 2.0f;
	m_fAxisHalfLen[2] = m_fAxisLen[2] / 2.0f;

	D3DXMatrixIdentity(&m_matWorldTM);
}

void cOBB::Update( D3DXMATRIXA16* pmatWorld )
{
	if(pmatWorld)
		m_matWorldTM = *pmatWorld;

	for (int i = 0; i < 3; ++i)
	{
		D3DXVec3TransformNormal(
			&m_vAxisDir[i],
			&m_vOrgAxisDir[i],
			&m_matWorldTM);
	}

	D3DXVec3TransformCoord(
		&m_vCenterPos,
		&m_vOrgCenterPos,
		&m_matWorldTM);
}

bool cOBB::IsCollision( cOBB* pOBB1, cOBB* pOBB2 )
{
	float cos[3][3];		
	float absCos[3][3];
	float dist[3];
	float r0, r1, r;

	const float cutOff = 0.999999f;
	bool existsParallelPair = false;

	D3DXVECTOR3 D = pOBB2->m_vCenterPos - pOBB1->m_vCenterPos;

	/// :  OBB1 �� 3 �� (a) �� �������� OBB2 �� 3��(b) �� �˻�
	for( int a = 0 ; a < 3 ; a++ )
	{
		for( int b = 0 ; b < 3 ; b++ )
		{
			// : OBB_A a ��� OBB_B b ���� cos ��
			cos[a][b] = D3DXVec3Dot( &pOBB1->m_vAxisDir[a], &pOBB2->m_vAxisDir[b] );

			/// : ������ ���� cos ���� ���밪
			absCos[a][b] = abs( cos[a][b] );


			/// : ������ �����̴� �̷��Ѱ�� AABB ����
			if( absCos[a][b] > cutOff )
				existsParallelPair = true;
		}


		/// : �簢�� ������ �߽� �Ÿ� ���͸� OBB1 �� a ������ ������ �Ÿ�
		dist[a] = D3DXVec3Dot( &pOBB1->m_vAxisDir[a], &D );
		r = abs( dist[a] );

		/// : OBB1 �� a ���� ����
		r0 = pOBB1->m_fAxisHalfLen[a];

		/// : OBB2 �� �������� OBB1 ��a ������ ������ ����
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[a][0] + 
			pOBB2->m_fAxisHalfLen[1] * absCos[a][1] +
			pOBB2->m_fAxisHalfLen[2] * absCos[a][2];

		if( r > r0 + r1 )
			return false;
	}

	/// OBB2 �� 3���� �������� OBB1 �� 3���� �˻�
	for( int b = 0 ; b < 3 ; b++ )
	{
		r = abs( D3DXVec3Dot( &pOBB2->m_vAxisDir[b], &D ) );
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[0][b] + 
			pOBB1->m_fAxisHalfLen[1] * absCos[1][b] +
			pOBB1->m_fAxisHalfLen[2] * absCos[2][b];

		r1 = pOBB2->m_fAxisHalfLen[b];

		if( r > r0 + r1 )
			return false;
	}


	if( existsParallelPair ) return true;

	/////////////////////////////////////////////////////////////////
	{	// : 3 x 3 �� 
		r = abs(dist[0] * cos[2][0] - dist[2] * cos[0][0]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[2][0] + pOBB1->m_fAxisHalfLen[2] * absCos[0][0];
		r1 = pOBB2->m_fAxisHalfLen[1] * absCos[1][2] + pOBB2->m_fAxisHalfLen[2] * absCos[1][1];
		if (r > r0 + r1)
			return false;

		r = abs(dist[0] * cos[2][1] - dist[2] * cos[0][1]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[2][1] + pOBB1->m_fAxisHalfLen[2] * absCos[0][1];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[1][2] + pOBB2->m_fAxisHalfLen[2] * absCos[1][0];
		if (r > r0 + r1)
			return false;

		r = abs(dist[0] * cos[2][2] - dist[2] * cos[0][2]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[2][2] + pOBB1->m_fAxisHalfLen[2] * absCos[0][2];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[1][1] + pOBB2->m_fAxisHalfLen[1] * absCos[1][0];
		if (r > r0 + r1)
			return false;

		/////////////////////////////////////////////////////////////////	 

		r = abs(dist[1] * cos[0][0] - dist[0] * cos[1][0]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[1][0] + pOBB1->m_fAxisHalfLen[1] * absCos[0][0];
		r1 = pOBB2->m_fAxisHalfLen[1] * absCos[2][2] + pOBB2->m_fAxisHalfLen[2] * absCos[2][1];
		if (r > r0 + r1)
			return false;

		r = abs(dist[1] * cos[0][1] - dist[0] * cos[1][1]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[1][1] + pOBB1->m_fAxisHalfLen[1] * absCos[0][1];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[2][2] + pOBB2->m_fAxisHalfLen[2] * absCos[2][0];
		if (r > r0 + r1)
			return false;


		r = abs(dist[1] * cos[0][2] - dist[0] * cos[1][2]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[1][2] + pOBB1->m_fAxisHalfLen[1] * absCos[0][2];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[2][1] + pOBB2->m_fAxisHalfLen[1] * absCos[2][0];
		if (r > r0 + r1)
			return false;
	
		/////////////////////////////////////////////////////////////////

		r = abs(dist[2] * cos[1][0] - dist[1] * cos[2][0]);
		r0 = pOBB1->m_fAxisHalfLen[1] * absCos[2][0] + pOBB1->m_fAxisHalfLen[2] * absCos[1][0];
		r1 = pOBB2->m_fAxisHalfLen[1] * absCos[0][2] + pOBB2->m_fAxisHalfLen[2] * absCos[0][1];
		if (r > r0 + r1)
			return false;



		r = abs(dist[2] * cos[1][1] - dist[1] * cos[2][1]);
		r0 = pOBB1->m_fAxisHalfLen[1] * absCos[2][1] + pOBB1->m_fAxisHalfLen[2] * absCos[1][1];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[0][2] + pOBB2->m_fAxisHalfLen[2] * absCos[0][0];
		if (r > r0 + r1)
			return false;



		r = abs(dist[2] * cos[1][2] - dist[1] * cos[2][2]);
		r0 = pOBB1->m_fAxisHalfLen[1] * absCos[2][2] + pOBB1->m_fAxisHalfLen[2] * absCos[1][2];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[0][1] + pOBB2->m_fAxisHalfLen[1] * absCos[0][0];
		if (r > r0 + r1)
			return false;
	} // << : 

	return true;
}

/// >> : �ڽ� �׸���.
void cOBB::OBBBox_Render(D3DCOLOR c)
{
	D3DXVECTOR3 pos1(-m_fAxisHalfLen[0],
		/*m_vCenterPos.y*/- m_fAxisHalfLen[1],
		-m_fAxisHalfLen[2]);
	D3DXVECTOR3 pos2(-m_fAxisHalfLen[0],
		/*m_vCenterPos.y*/+ m_fAxisHalfLen[1],
		-m_fAxisHalfLen[2]);
	D3DXVECTOR3 pos3(m_fAxisHalfLen[0],
		/*m_vCenterPos.y*/+ m_fAxisHalfLen[1],
		-m_fAxisHalfLen[2]);
	D3DXVECTOR3 pos4(m_fAxisHalfLen[0],
		/*m_vCenterPos.y*/- m_fAxisHalfLen[1],
		-m_fAxisHalfLen[2]);

	D3DXVECTOR3 pos5(-m_fAxisHalfLen[0],
		/*m_vCenterPos.y*/- m_fAxisHalfLen[1],
		m_fAxisHalfLen[2]);
	D3DXVECTOR3 pos6(-m_fAxisHalfLen[0],
		/*m_vCenterPos.y*/+ m_fAxisHalfLen[1],
		m_fAxisHalfLen[2]);
	D3DXVECTOR3 pos7(m_fAxisHalfLen[0],
		/*m_vCenterPos.y*/+ m_fAxisHalfLen[1],
		m_fAxisHalfLen[2]);
	D3DXVECTOR3 pos8(m_fAxisHalfLen[0],
		/*m_vCenterPos.y*/- m_fAxisHalfLen[1],
		m_fAxisHalfLen[2]);

	std::vector<ST_PC_VERTEX> vecVertex;
	vecVertex.push_back(ST_PC_VERTEX(pos1, c));
	vecVertex.push_back(ST_PC_VERTEX(pos2, c));
	vecVertex.push_back(ST_PC_VERTEX(pos2, c));
	vecVertex.push_back(ST_PC_VERTEX(pos3, c));
	vecVertex.push_back(ST_PC_VERTEX(pos3, c));
	vecVertex.push_back(ST_PC_VERTEX(pos4, c));
	vecVertex.push_back(ST_PC_VERTEX(pos4, c));
	vecVertex.push_back(ST_PC_VERTEX(pos1, c));
	vecVertex.push_back(ST_PC_VERTEX(pos1, c));
	vecVertex.push_back(ST_PC_VERTEX(pos5, c));
	vecVertex.push_back(ST_PC_VERTEX(pos2, c));
	vecVertex.push_back(ST_PC_VERTEX(pos6, c));
	vecVertex.push_back(ST_PC_VERTEX(pos3, c));
	vecVertex.push_back(ST_PC_VERTEX(pos7, c));
	vecVertex.push_back(ST_PC_VERTEX(pos4, c));
	vecVertex.push_back(ST_PC_VERTEX(pos8, c));
	vecVertex.push_back(ST_PC_VERTEX(pos5, c));
	vecVertex.push_back(ST_PC_VERTEX(pos6, c));
	vecVertex.push_back(ST_PC_VERTEX(pos6, c));
	vecVertex.push_back(ST_PC_VERTEX(pos7, c));
	vecVertex.push_back(ST_PC_VERTEX(pos7, c));
	vecVertex.push_back(ST_PC_VERTEX(pos8, c));
	vecVertex.push_back(ST_PC_VERTEX(pos8, c));
	vecVertex.push_back(ST_PC_VERTEX(pos5, c));

	g_pD3DDevice->SetTexture(NULL,NULL);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST,
		vecVertex.size() / 2,
		&vecVertex[0],
		sizeof(ST_PC_VERTEX));
}

void cOBB::SetCenter(D3DXMATRIXA16 & matSRT)
{
	D3DXMatrixInverse(&matSRT, 0, &matSRT);
	D3DXVec3TransformCoord(&m_vOrgCenterPos, &m_vOrgCenterPos, &matSRT);
}
