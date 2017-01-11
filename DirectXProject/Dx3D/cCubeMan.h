#pragma once

#include "cCharacter.h"

class cCubeNode;

class cCubeMan : public cCharacter
{
protected:
	cCubeNode*			m_pRoot;
	LPDIRECT3DTEXTURE9	m_pTexture;
	D3DMATERIAL9		m_stMtl;

public:
	cCubeMan(void);
	virtual ~cCubeMan(void);

	virtual void Setup() override;
	virtual void Update(iMap* pMap) override;
	virtual void Render() override;
};

