#pragma once
#include "cController.h"

class cAIController
	: public cController
{
public:
	cAIController();
	~cAIController();

	void Setup(float moveSpeed) override;
	void Update(D3DXVECTOR3& camAngle, OUT D3DXVECTOR3& vDirection, OUT D3DXVECTOR3& vPosition) override;
};

