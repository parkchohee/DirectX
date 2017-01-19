#include "stdafx.h"
#include "cGun.h"
#include "cSkinnedMesh.h"
#include "cBullet.h"

cGun::cGun()
	: m_pGun(NULL)
	, m_pvTarget(NULL)
	, m_fAttackPower(1.f)
	, m_fAttackRange(30.f)
	, m_fAttackSpeed(1.f)
	, m_nMaxBullet(10)
	, m_nCurrentBullet(10)
{
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
}

void cGun::Render()
{
	if (m_pGun)
		m_pGun->UpdateAndRender();
	
	for (size_t i = 0; i < m_pvBullet.size(); i++)
	{
		m_pvBullet[i]->Render();
	}
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
		m_pWorldTM = matWorld * *m_pGun->getLocalMatrix(name);
		m_pGun->SetTransform(&m_pWorldTM);
	}
}

void cGun::Fire(D3DXVECTOR3 & vDirection, D3DXVECTOR3 & vPosition)
{
 	//m_pGun->SetAnimationIndex(7);
	m_pGun->PlayOneShot(3,0,0);
	if (m_pvBullet.size() < m_nMaxBullet)
	{
		cBullet* bullet = new cBullet;
		bullet->Setup(vDirection, vPosition);
		m_pvBullet.push_back(bullet);
	}

}
