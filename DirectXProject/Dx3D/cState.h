#pragma once

class cGameObject;

class cState;
class iStateDelegate
{
public:
	virtual void OnStateFinish(cState* pSender) = 0;
};

class cState : public cObject
{
protected:
	SYNTHESIZE(cGameObject*, m_pTarget, Target);
	SYNTHESIZE(iStateDelegate*, m_pDelegate, Delegate);

public:
	cState();
	virtual ~cState();

	virtual void Start() PURE;
	virtual void Update() PURE;
};

