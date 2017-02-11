#include "stdafx.h"
#include "cVideo.h"


cVideo::cVideo()
	: hWndVideo(NULL)
{
}


cVideo::~cVideo()
{
}

bool cVideo::VideoState()
{
	char str[10];
	switch (MCIWndGetMode(hWndVideo, str, sizeof(str)))
	{
	case MCI_MODE_STOP:
		return false;
	case MCI_MODE_PLAY:
		return true;
	}
}

void cVideo::Release(void)
{
	MCIWndClose(hWndVideo);
	MCIWndDestroy(hWndVideo);
	hWndVideo = NULL;
}

void cVideo::Play(char * fileName, int x, int y, int width, int height)
{
	if (hWndVideo)
	{
		MCIWndClose(hWndVideo);
		MCIWndDestroy(hWndVideo);
		hWndVideo = NULL;
	}

	hWndVideo = MCIWndCreate(g_hWnd, hInst,
		MCIWNDF_NOTIFYANSI | MCIWNDF_NOMENU |
		MCIWNDF_NOTIFYALL | MCIWNDF_NOPLAYBAR, fileName);

	if (hWndVideo)
	{
		SetWindowPos(hWndVideo, NULL, x, y, width, height, SWP_NOZORDER | SWP_NOMOVE);
		MCIWndPlay(hWndVideo);
		MCIWndSetVolume(hWndVideo, 0);
	}
}

void cVideo::Stop()
{
	if (!hWndVideo) return;

	if (hWndVideo)
	{
		MCIWndPlay(hWndVideo);
	}
}

void cVideo::Pause()
{
	if (!hWndVideo) return;

	if (hWndVideo)
	{
		MCIWndPause(hWndVideo);
	}
}

void cVideo::Resume()
{
	if (!hWndVideo) return;

	if (hWndVideo)
	{
		MCIWndResume(hWndVideo);
	}
}

void cVideo::Replay()
{
	if (!VideoState())
		MCIWndPlay(hWndVideo);
}

void cVideo::SetVolume(int volume)
{
	MCIWndSetVolume(hWndVideo, volume);
}
