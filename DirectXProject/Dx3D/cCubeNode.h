#pragma once

#include "cCubePNT.h"

class cCubeNode : public cCubePNT
{
protected:
	float					m_fRotX;
	D3DXVECTOR3				m_vLocalPos;

	D3DXMATRIXA16			m_matLocalTM;
	D3DXMATRIXA16			m_matWorldTM;
	std::vector<cCubeNode*>	m_vecChild;
	
	SYNTHESIZE(D3DXMATRIXA16*, m_pParentWorldTM, ParentWorldTM);
	SYNTHESIZE(float, m_fRotDeltaX, RotDeltaX);

public:
	cCubeNode(void);
	virtual ~cCubeNode(void);

	virtual void AddChild(cCubeNode* pChild);
	virtual void Destroy();

	// cCubePNT override
	virtual void Setup() override;
	virtual void Update() override;
	virtual void Render() override;
};

