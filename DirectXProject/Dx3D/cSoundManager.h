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
	//typedef std::map< std::string, FMOD::Sound* >				MAP_SOUND;		//Sound �� ������ Map typedef
	//typedef std::map< std::string, FMOD::Sound* >::iterator		MAP_SOUND_ITER;

	//FMOD::System*			m_pSystem;		//���� System ��ü

	//MAP_SOUND				m_mapSounds;					//ȿ���� ��
	//MAP_SOUND				m_mapBGMs;						//BGM ��
	//MAP_SOUND				m_map3DSounds;


	//FMOD::ChannelGroup*		m_pSoundChannelGroup;			//���� ä�� �׷�
	//FMOD::ChannelGroup*		m_pBGMChannelGroup;				//BGM ä�� �׷�


public:

	//void Setup();
	//void Destroy();


	////�� ������Ʈ ���� SoundSystem �� ������Ʈ �ؾ� �Ѵ�.
	//void UpdateSound(void);

	////BGM �߰�
	//FMOD::Sound* AddBGM(std::string keyName, std::string fileName);

	////ȿ���� �߰�
	//FMOD::Sound* AddSound(std::string keyName, std::string fileName);

	////3D ���� �߰�
	//FMOD::Sound* Add3DSound(std::string keyName, std::string fileName, float minDistance, float maxDistance, bool bLoop);



	////BGM ���
	//void PlayBGM(std::string keyName);
	//void PlayBGM(FMOD::Sound* pSound);

	////ȿ����
	//void PlaySound(std::string keyName);
	//void PlaySound(FMOD::Sound* pSound);


	////Ű �̸����� ���� ��ü�� ��´�.
	//FMOD::Sound* GetBGM(const char* keyName);
	//FMOD::Sound* GetSound(const char* keyName);
	//FMOD::Sound* Get3DSound(const char* keyName);

	//FMOD::System* GetSoundSystem() {
	//	return m_pSystem;
	//}


	////�÷��� �ǰ� �ִ� ���� ��� ����
	//void AllStop(void);

	////�÷��� �ǰ� �ִ� ���� �Ͻ� ���� �Ǵ� ��� ( ��� ��� )
	//void AllPauseAndResume(void);

	////���� ����
	//void SetVolum(float volume);

	////��ġ����
	//void SetPitch(float pitch);

	////�¿����� ( -1 �̸� ���� �̺��� 0 �̸� ���� 1 �̸� ������ )
	//void SetPan(float pan);

};

