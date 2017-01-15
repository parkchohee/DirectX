#pragma once
#include "cController.h"

class cPlayerController 
	: public cController
{
public:
	cPlayerController();
	~cPlayerController();

	void Setup() override;
	void Update(D3DXVECTOR3& camAngle, OUT D3DXVECTOR3& vDirection, OUT D3DXVECTOR3& vPosition) override;
};

