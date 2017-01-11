#pragma once

#include "cCubeNode.h"

class cRArm : public cCubeNode
{
public:
	cRArm(void);
	~cRArm(void);

	virtual void Setup() override;
};

