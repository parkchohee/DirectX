#include "stdafx.h"
#include "cGun.h"
#include "cSkinnedMesh.h"
#include "cBullet.h"
#include "cEffect.h"

cGun::cGun()
	: m_pGun(NULL)
	, m_pvTarget(NULL)
	, m_fAttackPower(1.f)
	, m_fAttackRange(10.f)
	, m_fAttackSpeed(1.f)
	, m_nMaxBullet(10)
	, m_nCurrentBullet(10)
{
	D3DXMatrixIdentity(&m_pTrans);
}


cGun::~cGun()
{
	for each (auto p in m_pvBullet)
		SAFE_RELEASE(p);

	SAFE_DELETE(m_pGun);
}

void cGun::Setup(D3DXVECTOR3* pvTarget, char* szFolder, char* szFilename)
{
	m_pvTarget = pvTarget;

	m_pGun = new cSkinnedMesh(szFolder, szFilename);

}

void cGun::Update()
{
	for (size_t i = 0; i < m_pvBullet.size(); i++)
	{
		m_pvBullet[i]->Update();
		if(m_pvBullet[i]->GetMoveDistance() > m_fAttackRange)
		{
			SAFE_RELEASE(m_pvBullet[i]);
			m_pvBullet.erase(m_pvBullet.begin());
			break;
		}
	}

	for (size_t i = 0; i < m_pvEffect.size(); )
	{
		if (!m_pvEffect[i]->GetPlay())
		{
			m_pvEffect[i]->Destroy();
			m_pvEffect.erase(m_pvEffect.begin() + i);
			continue;
		}

		m_pvEffect[i]->Update();
		i++;
	}

}

void cGun::Render()
{
	if (m_pGun)
		m_pGun->UpdateAndRender();

	for each (auto p in m_pvEffect)
		p->Render();

	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, true);

	/*for (size_t i = 0; i < m_pvBullet.size(); i++)
	{
		m_pvBullet[i]->Render();
	}*/

}

void cGun::SetParentWorldMatrix(D3DXMATRIXA16* matWorld)
{
	if (m_pGun)
		m_pGun->SetParent(matWorld);
}

void cGun::SetWorldMatrix(D3DXMATRIXA16* matWorld)
{
	if (m_pGun)
		m_pGun->SetTransform(matWorld);
}
void cGun::SetWorldMatrixByBoneName(D3DXMATRIXA16 * matRot, char * name)
{
	if (m_pGun)
	{
		D3DXMATRIXA16 matWorld = *matRot;
		m_pTrans = *m_pGun->getWorldMatrix(name);
		D3DXMatrixInverse(&m_pTransInv, 0, &m_pTrans);
		m_pWorldTM = m_pTrans * matWorld * m_pTransInv * *m_pGun->getWorldMatrix(name);
		m_pGun->SetTransform(&m_pWorldTM);
	}
}

void cGun::Fire(D3DXVECTOR3 & vDirection, D3DXMATRIXA16 & matWorld)
{
	m_nCurrentBullet--;

	D3DXMATRIXA16 matPos = matWorld;

	if (*m_pGun->getLocalMatrix("tip") != NULL)
		matPos = *m_pGun->getLocalMatrix("tip") * matPos;
	else if(*m_pGun->getLocalMatrix("tag_barrell") != NULL)
		matPos = *m_pGun->getLocalMatrix("tag_barrell") * matPos;
	
	// �ѱ����� �������� ����
	//D3DXMATRIXA16 matPos = *m_pGun->getLocalMatrix("tip") * matWorld;
	D3DXVECTOR3 vecPos(0,0,0);
	D3DXVec3TransformCoord(&vecPos, &vecPos, &matPos);

	if (m_pvBullet.size() < m_nMaxBullet)
	{
		cBullet* bullet = new cBullet;
		bullet->Setup(vDirection, vecPos);
		m_pvBullet.push_back(bullet);
	}

	cEffect* test = new cEffect;
	test->Setup("Effect/test4.tga", 1, 1, 2, 1);
	test->SetPosition(D3DXVECTOR3(vecPos.x, vecPos.y, vecPos.z));
	m_pvEffect.push_back(test);
}

void cGun::Reload()
{
	m_nCurrentBullet = m_nMaxBullet;
	// reload �ִϸ��̼� �ѹ��� ����
	m_pGun->PlayOneShot(2, 0, 0);
}

void cGun::RemoveBullet(int bulletIndex)
{
	m_pvBullet.erase(m_pvBullet.begin() + bulletIndex);
}

cSkinnedMesh * cGun::GetGunMesh()
{
	return m_pGun;
}

