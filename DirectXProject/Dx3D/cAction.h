#pragma once

class cAction;

class iActionDelegate
{
public:
	virtual void OnActionFinish(cAction* pSender) = 0;
};

class cGameObject;

class cAction : public cObject
{
protected:
	SYNTHESIZE(cGameObject*, m_pOwner, Owner);
	SYNTHESIZE(iActionDelegate*, m_pDelegate, Delegate);

public:
	cAction(void);
	virtual ~cAction(void);

	virtual void Start() = 0;
	virtual void Update() = 0;
};

