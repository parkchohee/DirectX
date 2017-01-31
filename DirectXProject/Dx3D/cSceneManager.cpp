#include "stdafx.h"
#include "cSceneManager.h"
#include "cScene.h"


cSceneManager::cSceneManager()
	: m_pNowScene(NULL)
	, m_pReleaseScene(NULL)
{
}


cSceneManager::~cSceneManager()
{
}

DWORD CALLBACK LoadingThread(LPVOID lpParam)
{
	cScene* pScene = (cScene*)lpParam;

	pScene->Setup();

	g_pSceneManager->m_pNowScene = pScene;

	return 0;
}

void cSceneManager::Setup()
{
}

void cSceneManager::Update(float timeDelta)
{
}

void cSceneManager::Render()
{
	if (m_pNowScene != NULL) {
		m_pNowScene->Render();
	}
}

void cSceneManager::AddScene(std::string sceneName, cScene * pScene)
{
	MAP_SCENE::iterator  iter = m_Scenes.find(sceneName);

	if (iter == m_Scenes.end()) {
		m_Scenes.insert(std::make_pair(sceneName, pScene));
	}
}

void cSceneManager::AddLoadingScene(std::string sceneName, cScene * pScene)
{
	MAP_SCENE::iterator  iter = m_LoadingScenes.find(sceneName);

	if (iter == m_LoadingScenes.end()) {
		pScene->Setup();
		m_LoadingScenes.insert(std::make_pair(sceneName, pScene));
	}
}

void cSceneManager::ChangeScene(std::string sceneName)
{
}

void cSceneManager::ChangeSceneWithLoading(std::string sceneName, std::string loadingSceneName)
{
}
