#pragma once
#define g_pSoundManager cSoundManager::GetInstance()
//
//#include "FMOD/inc/fmod.hpp"
//#pragma comment( lib, "FMOD/lib/fmod_vc.lib")

#define SOUND_CHANNEL_NUM		128

class cSoundManager
{
	SINGLETONE(cSoundManager);
private:
	//typedef std::map< std::string, FMOD::Sound* >				MAP_SOUND;		//Sound 를 저장할 Map typedef
	//typedef std::map< std::string, FMOD::Sound* >::iterator		MAP_SOUND_ITER;

	//FMOD::System*			m_pSystem;		//사운드 System 객체

	//MAP_SOUND				m_mapSounds;					//효과음 맵
	//MAP_SOUND				m_mapBGMs;						//BGM 맵
	//MAP_SOUND				m_map3DSounds;


	//FMOD::ChannelGroup*		m_pSoundChannelGroup;			//사운드 채널 그룹
	//FMOD::ChannelGroup*		m_pBGMChannelGroup;				//BGM 채널 그룹


public:

	//void Setup();
	//void Destroy();


	////매 업데이트 마다 SoundSystem 을 업데이트 해야 한다.
	//void UpdateSound(void);

	////BGM 추가
	//FMOD::Sound* AddBGM(std::string keyName, std::string fileName);

	////효과음 추가
	//FMOD::Sound* AddSound(std::string keyName, std::string fileName);

	////3D 사운드 추가
	//FMOD::Sound* Add3DSound(std::string keyName, std::string fileName, float minDistance, float maxDistance, bool bLoop);



	////BGM 재생
	//void PlayBGM(std::string keyName);
	//void PlayBGM(FMOD::Sound* pSound);

	////효과음
	//void PlaySound(std::string keyName);
	//void PlaySound(FMOD::Sound* pSound);


	////키 이름으로 사운드 객체를 얻는다.
	//FMOD::Sound* GetBGM(const char* keyName);
	//FMOD::Sound* GetSound(const char* keyName);
	//FMOD::Sound* Get3DSound(const char* keyName);

	//FMOD::System* GetSoundSystem() {
	//	return m_pSystem;
	//}


	////플레이 되고 있는 사운드 모두 중지
	//void AllStop(void);

	////플레이 되고 있는 사운드 일시 정시 또는 재생 ( 토글 방식 )
	//void AllPauseAndResume(void);

	////볼륨 조정
	//void SetVolum(float volume);

	////피치조정
	//void SetPitch(float pitch);

	////좌우조정 ( -1 이면 왼쪽 이빠이 0 이면 센터 1 이며 오른쪽 )
	//void SetPan(float pan);

};

