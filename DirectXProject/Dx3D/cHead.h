#pragma once

#include "cCubeNode.h"

class cHead : public cCubeNode
{
public:
	cHead(void);
	~cHead(void);

	virtual void Setup() override;
};

