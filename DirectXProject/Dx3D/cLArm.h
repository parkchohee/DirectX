#pragma once

#include "cCubeNode.h"

class cLArm : public cCubeNode
{
public:
	cLArm(void);
	~cLArm(void);

	virtual void Setup() override;
};

