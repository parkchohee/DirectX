#pragma once
#include "cScene.h"
#include "cVideo.h"

class cVideoScene : public cScene
{
private:
	cVideo*			m_pintroVideo;
	bool			m_bstartBackbuffer;
	bool			m_binitComplete;
public:
	cVideoScene();
	virtual ~cVideoScene();

	virtual void Setup();
	virtual void Update();
	virtual void Render();
};

