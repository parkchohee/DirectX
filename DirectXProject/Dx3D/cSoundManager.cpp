#include "stdafx.h"
#include "cSoundManager.h"


cSoundManager::cSoundManager()
{
}


cSoundManager::~cSoundManager()
{
}
//
//void cSoundManager::Setup()
//{
//	//Sound System 생성
//	FMOD::System_Create(&m_pSystem);
//
//	//Sound System 초기화
//	m_pSystem->init(
//		SOUND_CHANNEL_NUM,			//사운드 채널수 셋팅
//		FMOD_INIT_NORMAL,
//		NULL);
//
//	//Sound 채널 그룹 생성
//	m_pSystem->createChannelGroup(
//		"BGM",					//생성된 채널그룹 이름
//		&m_pBGMChannelGroup		//생성된 채널그룹을 받아올 FMOD::ChannelGroup 대한 더블포인터.
//	);
//
//
//	m_pSystem->createChannelGroup(
//		"Sound",
//		&m_pSoundChannelGroup);
//
//	return ;
//}
//void cSoundManager::Destroy(void)
//{
//	//사용된 Sound 해재
//	MAP_SOUND_ITER iter;
//	for (iter = m_mapBGMs.begin(); iter != m_mapBGMs.end(); ++iter)
//		iter->second->release();
//	m_mapBGMs.clear();
//
//	for (iter = m_mapSounds.begin(); iter != m_mapSounds.end(); ++iter)
//		iter->second->release();
//	m_mapSounds.clear();
//
//	for (iter = m_map3DSounds.begin(); iter != m_map3DSounds.end(); ++iter)
//		iter->second->release();
//	m_map3DSounds.clear();
//
//
//	//Sound 채널그룹 해제
//	m_pSoundChannelGroup->release();
//	m_pBGMChannelGroup->release();
//
//	//Sound System 해제
//	m_pSystem->release();
//	m_pSystem->close();
//}
//
////매 업데이트 마다 SoundSystem 을 업데이트 해야 한다.
//void cSoundManager::UpdateSound(void)
//{
//	//Sound 들의 채널 업데이트가 이루어져야 
//	//플레이가 끝난 Sound 가 채널에서 빠지고
//	//볼륨조정이 일어났다면 볼륨을 바꾸는등 여러가지 업데이가 이루어진다.
//	//pause 나 resume 마찬가지임...
//	//이는 게임 loop 에서 Update 때 해주자
//	m_pSystem->update();
//}
//
//
////BGM 추가
//FMOD::Sound* cSoundManager::AddBGM(std::string keyName, std::string fileName)
//{
//	//이미 존재하는 Key 라면 추가 로딩 되지 않는다.
//	MAP_SOUND_ITER iter = m_mapBGMs.find(keyName);
//	if (iter != m_mapBGMs.end())
//		return iter->second;
//
//	//추가
//	FMOD::Sound* pNewSound = NULL;
//
//	//BGM 같은 것은 Stream 방식으로 얻는다. ( stream 방식의 로딩은 플레이 비용이 좀들지만 메모리를 덜쓴다 자주 안바뀌고 플레이되는 Sound 를 로딩할때 쓰자 )
//	m_pSystem->createStream(
//		fileName.c_str(),
//		FMOD_2D,
//		NULL,
//		&pNewSound);
//
//	if (pNewSound != NULL)
//	{
//		//BGM 맵에 추가
//		m_mapBGMs.insert(std::make_pair(keyName, pNewSound));
//	}
//
//	return pNewSound;
//
//}
//
////효과음 추가
//FMOD::Sound* cSoundManager::AddSound(std::string keyName, std::string fileName)
//{
//	//이미 존재하는 Key 라면 추가 로딩 되지 않는다.
//	MAP_SOUND_ITER iter = m_mapSounds.find(keyName);
//	if (iter != m_mapSounds.end()) return iter->second;
//
//	//추가
//	FMOD::Sound* pNewSound = NULL;
//	m_pSystem->createSound(				//(createSound 방식의 로딩은 로딩되는 사운드 정보를 메모리에 적재하고 바로가져다 쓰기때문에 플레이의 비용이 적지만 메모리사용이 있다 때문에 효과음같이 자주 터지고 용량이 작은 Sound 를 로딩할때 쓰자 )
//		fileName.c_str(),
//		FMOD_2D | FMOD_LOOP_OFF,
//		NULL, &pNewSound);
//
//	if (pNewSound != NULL)
//		m_mapSounds.insert(std::make_pair(keyName, pNewSound));
//
//	return pNewSound;
//}
//
////3D 사운드 추가
//FMOD::Sound*  cSoundManager::Add3DSound(std::string keyName, std::string fileName, float minDistance, float maxDistance, bool bLoop)
//{
//	//이미 존재하는 Key 라면 추가 로딩 되지 않는다.
//	MAP_SOUND_ITER iter = m_map3DSounds.find(keyName);
//	if (iter != m_map3DSounds.end()) return iter->second;
//
//	//FMOD_3D_LINEARROLLOFF 옵션으로 쓰면 MaxMin 거리내에서 선형으로 볼륨이빠진다.
//
//	FMOD::Sound* pNewSound = NULL;
//	m_pSystem->createSound(
//		fileName.c_str(),
//		FMOD_3D | FMOD_3D_LINEARROLLOFF,				//3D 사운들로 쓰겠다..
//		NULL,
//		&pNewSound);
//
//	if (pNewSound == NULL)
//	{
//		return NULL;
//	}
//
//	//루프가 된다라면.....
//	if (bLoop)
//		pNewSound->setMode(FMOD_LOOP_NORMAL);
//
//
//
//
//	//로딩된 사운드의 최대 최소값 거리 설정
//	pNewSound->set3DMinMaxDistance(minDistance, maxDistance);
//
//	//맵에 추가.
//	m_map3DSounds.insert(std::make_pair(keyName, pNewSound));
//
//
//
//	//FMOD_VECTOR vec[] = { { 0, 0, 0 }, { 1, 1, 1 } };
//	//pNewSound->set3DCustomRolloff(NULL, 0);
//
//	return pNewSound;
//
//}
//
//
////BGM 재생
//void cSoundManager::PlayBGM(std::string keyName)
//{
//	MAP_SOUND_ITER iter = m_mapBGMs.find(keyName);
//	if (iter != m_mapBGMs.end()) {
//		PlayBGM(iter->second);
//	}
//}
//void cSoundManager::PlayBGM(FMOD::Sound* pSound)
//{
//	//BGM 은 보통 하나만 출력된다 따라서 기존에 플레이 되고 있는 사운드는 Stop하자
//	m_pBGMChannelGroup->stop();
//
//	m_pSystem->playSound(
//		pSound,
//		m_pBGMChannelGroup, //BGM 채널 그룹에서 실행이 된다.
//		false,
//		NULL				//플레이된 채널
//	);
//}
//
////효과음
//void cSoundManager::PlaySound(std::string keyName)
//{
//	MAP_SOUND_ITER iter = m_mapSounds.find(keyName);
//	if (iter != m_mapSounds.end()) {
//		PlaySound(iter->second);
//	}
//}
//void cSoundManager::PlaySound(FMOD::Sound* pSound)
//{
//	m_pSystem->playSound(
//		pSound,
//		m_pSoundChannelGroup, //Sound 채널 그룹에서 실행이 된다.
//		false,
//		NULL);
//}
//
//
////키 이름으로 사운드 객체를 얻는다.
//FMOD::Sound* cSoundManager::GetBGM(const char* keyName)
//{
//	MAP_SOUND_ITER iter = m_mapBGMs.find(keyName);
//	if (iter == m_mapBGMs.end()) return NULL;
//
//	return iter->second;
//}
//FMOD::Sound* cSoundManager::GetSound(const char* keyName)
//{
//	MAP_SOUND_ITER iter = m_mapSounds.find(keyName);
//	if (iter == m_mapSounds.end()) return NULL;
//
//	return iter->second;
//}
//FMOD::Sound* cSoundManager::Get3DSound(const char* keyName)
//{
//	MAP_SOUND_ITER iter = m_map3DSounds.find(keyName);
//	if (iter == m_map3DSounds.end()) return NULL;
//
//	return iter->second;
//}
//
////플레이 되고 있는 사운드 모두 중지
//void cSoundManager::AllStop(void)
//{
//	m_pSoundChannelGroup->stop();		//Sound Channel 그룹에서 플레이 되고있는거 싹다 stop
//	m_pBGMChannelGroup->stop();			//BGM Channel 그룹에서 플레이 되고있는거 싹다 stop
//}
//
////플레이 되고 있는 사운드 일시 정시 또는 재생 ( 토글 방식 )
//void cSoundManager::AllPauseAndResume(void)
//{
//	bool bSoundPause;
//	m_pSoundChannelGroup->getPaused(&bSoundPause);		//Sound 일시정지 상태인지를 얻는다.
//
//	bool bBGMPause;
//	m_pBGMChannelGroup->getPaused(&bBGMPause);			//BGM 일시정지 상태인지를 얻는다.
//
//	m_pSoundChannelGroup->setPaused(!bSoundPause);
//	m_pBGMChannelGroup->setPaused(!bBGMPause);
//}
//
////볼륨 조정
//void cSoundManager::SetVolum(float volume)
//{
//	m_pSoundChannelGroup->setVolume(volume);
//	m_pBGMChannelGroup->setVolume(volume);
//}
//
////피치조정
//void cSoundManager::SetPitch(float pitch)
//{
//	m_pSoundChannelGroup->setPitch(pitch);
//	m_pBGMChannelGroup->setPitch(pitch);
//}
//
////피치조정
//void cSoundManager::SetPan(float pan)
//{
//	m_pSoundChannelGroup->setPan(pan);
//	m_pBGMChannelGroup->setPan(pan);
//}
