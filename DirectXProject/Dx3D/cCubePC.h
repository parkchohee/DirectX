#pragma once

#include "cCharacter.h"

class cCubePC : public cCharacter
{
private:
	std::vector<ST_PC_VERTEX>	m_vecVertex;

public:
	cCubePC(void);
	~cCubePC(void);

	virtual void Setup() override;
	virtual void Update(iMap* pMap) override;
	virtual void Render() override;
};

