#pragma once
#include "cController.h"

class cAIController
	: public cController
{
	float			m_fAngleX;
	float			m_fAngleY;

public:
	cAIController();
	~cAIController();

	void Setup(float moveSpeed) override;
	void Update(OUT D3DXVECTOR3& camAngle, OUT D3DXVECTOR3& vDirection, OUT D3DXVECTOR3& vPosition, iMap* pHeightMap = NULL, iMap* pTextMap = NULL) override;
};

