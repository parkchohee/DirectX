#pragma once

#include "cCubeNode.h"

class cBody : public cCubeNode
{
public:
	cBody(void);
	~cBody(void);

	virtual void Setup() override;
};

