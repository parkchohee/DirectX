#pragma once

#include "cAction.h"

class cActionSeq
	: public cAction
	, public iActionDelegate
{
protected:
	std::list<cAction*>::iterator	m_itCurrentAction;
	std::list<cAction*>				m_lstAction;

public:
	cActionSeq(void);
	virtual ~cActionSeq(void);

	virtual void AddAction(cAction* pAction);

	// cAction override
	virtual void Start() override;
	virtual void Update() override;

	// iActionDelegate override
	virtual void OnActionFinish(cAction* pSender) override;
};

