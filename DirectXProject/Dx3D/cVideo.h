#pragma once
#include "stdafx.h"
#include <Vfw.h>
#include "cScene.h"

class cVideo : public cScene
{
private:
	HWND hWndVideo;
	RECT _rc;
public:
	cVideo();
	~cVideo();

	bool VideoState();
	void Release(void);

	void Play(char* fileName, int x, int y, int width, int height);
	void Stop();
	void Pause();
	void Resume();
	void Replay();
	void SetVolume(int volume);
};

