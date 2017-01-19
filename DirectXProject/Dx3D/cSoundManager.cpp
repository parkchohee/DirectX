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
//	//Sound System ����
//	FMOD::System_Create(&m_pSystem);
//
//	//Sound System �ʱ�ȭ
//	m_pSystem->init(
//		SOUND_CHANNEL_NUM,			//���� ä�μ� ����
//		FMOD_INIT_NORMAL,
//		NULL);
//
//	//Sound ä�� �׷� ����
//	m_pSystem->createChannelGroup(
//		"BGM",					//������ ä�α׷� �̸�
//		&m_pBGMChannelGroup		//������ ä�α׷��� �޾ƿ� FMOD::ChannelGroup ���� ����������.
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
//	//���� Sound ����
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
//	//Sound ä�α׷� ����
//	m_pSoundChannelGroup->release();
//	m_pBGMChannelGroup->release();
//
//	//Sound System ����
//	m_pSystem->release();
//	m_pSystem->close();
//}
//
////�� ������Ʈ ���� SoundSystem �� ������Ʈ �ؾ� �Ѵ�.
//void cSoundManager::UpdateSound(void)
//{
//	//Sound ���� ä�� ������Ʈ�� �̷������ 
//	//�÷��̰� ���� Sound �� ä�ο��� ������
//	//���������� �Ͼ�ٸ� ������ �ٲٴµ� �������� �����̰� �̷������.
//	//pause �� resume ����������...
//	//�̴� ���� loop ���� Update �� ������
//	m_pSystem->update();
//}
//
//
////BGM �߰�
//FMOD::Sound* cSoundManager::AddBGM(std::string keyName, std::string fileName)
//{
//	//�̹� �����ϴ� Key ��� �߰� �ε� ���� �ʴ´�.
//	MAP_SOUND_ITER iter = m_mapBGMs.find(keyName);
//	if (iter != m_mapBGMs.end())
//		return iter->second;
//
//	//�߰�
//	FMOD::Sound* pNewSound = NULL;
//
//	//BGM ���� ���� Stream ������� ��´�. ( stream ����� �ε��� �÷��� ����� �������� �޸𸮸� ������ ���� �ȹٲ�� �÷��̵Ǵ� Sound �� �ε��Ҷ� ���� )
//	m_pSystem->createStream(
//		fileName.c_str(),
//		FMOD_2D,
//		NULL,
//		&pNewSound);
//
//	if (pNewSound != NULL)
//	{
//		//BGM �ʿ� �߰�
//		m_mapBGMs.insert(std::make_pair(keyName, pNewSound));
//	}
//
//	return pNewSound;
//
//}
//
////ȿ���� �߰�
//FMOD::Sound* cSoundManager::AddSound(std::string keyName, std::string fileName)
//{
//	//�̹� �����ϴ� Key ��� �߰� �ε� ���� �ʴ´�.
//	MAP_SOUND_ITER iter = m_mapSounds.find(keyName);
//	if (iter != m_mapSounds.end()) return iter->second;
//
//	//�߰�
//	FMOD::Sound* pNewSound = NULL;
//	m_pSystem->createSound(				//(createSound ����� �ε��� �ε��Ǵ� ���� ������ �޸𸮿� �����ϰ� �ٷΰ����� ���⶧���� �÷����� ����� ������ �޸𸮻���� �ִ� ������ ȿ�������� ���� ������ �뷮�� ���� Sound �� �ε��Ҷ� ���� )
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
////3D ���� �߰�
//FMOD::Sound*  cSoundManager::Add3DSound(std::string keyName, std::string fileName, float minDistance, float maxDistance, bool bLoop)
//{
//	//�̹� �����ϴ� Key ��� �߰� �ε� ���� �ʴ´�.
//	MAP_SOUND_ITER iter = m_map3DSounds.find(keyName);
//	if (iter != m_map3DSounds.end()) return iter->second;
//
//	//FMOD_3D_LINEARROLLOFF �ɼ����� ���� MaxMin �Ÿ������� �������� �����̺�����.
//
//	FMOD::Sound* pNewSound = NULL;
//	m_pSystem->createSound(
//		fileName.c_str(),
//		FMOD_3D | FMOD_3D_LINEARROLLOFF,				//3D ����� ���ڴ�..
//		NULL,
//		&pNewSound);
//
//	if (pNewSound == NULL)
//	{
//		return NULL;
//	}
//
//	//������ �ȴٶ��.....
//	if (bLoop)
//		pNewSound->setMode(FMOD_LOOP_NORMAL);
//
//
//
//
//	//�ε��� ������ �ִ� �ּҰ� �Ÿ� ����
//	pNewSound->set3DMinMaxDistance(minDistance, maxDistance);
//
//	//�ʿ� �߰�.
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
////BGM ���
//void cSoundManager::PlayBGM(std::string keyName)
//{
//	MAP_SOUND_ITER iter = m_mapBGMs.find(keyName);
//	if (iter != m_mapBGMs.end()) {
//		PlayBGM(iter->second);
//	}
//}
//void cSoundManager::PlayBGM(FMOD::Sound* pSound)
//{
//	//BGM �� ���� �ϳ��� ��µȴ� ���� ������ �÷��� �ǰ� �ִ� ����� Stop����
//	m_pBGMChannelGroup->stop();
//
//	m_pSystem->playSound(
//		pSound,
//		m_pBGMChannelGroup, //BGM ä�� �׷쿡�� ������ �ȴ�.
//		false,
//		NULL				//�÷��̵� ä��
//	);
//}
//
////ȿ����
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
//		m_pSoundChannelGroup, //Sound ä�� �׷쿡�� ������ �ȴ�.
//		false,
//		NULL);
//}
//
//
////Ű �̸����� ���� ��ü�� ��´�.
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
////�÷��� �ǰ� �ִ� ���� ��� ����
//void cSoundManager::AllStop(void)
//{
//	m_pSoundChannelGroup->stop();		//Sound Channel �׷쿡�� �÷��� �ǰ��ִ°� �ϴ� stop
//	m_pBGMChannelGroup->stop();			//BGM Channel �׷쿡�� �÷��� �ǰ��ִ°� �ϴ� stop
//}
//
////�÷��� �ǰ� �ִ� ���� �Ͻ� ���� �Ǵ� ��� ( ��� ��� )
//void cSoundManager::AllPauseAndResume(void)
//{
//	bool bSoundPause;
//	m_pSoundChannelGroup->getPaused(&bSoundPause);		//Sound �Ͻ����� ���������� ��´�.
//
//	bool bBGMPause;
//	m_pBGMChannelGroup->getPaused(&bBGMPause);			//BGM �Ͻ����� ���������� ��´�.
//
//	m_pSoundChannelGroup->setPaused(!bSoundPause);
//	m_pBGMChannelGroup->setPaused(!bBGMPause);
//}
//
////���� ����
//void cSoundManager::SetVolum(float volume)
//{
//	m_pSoundChannelGroup->setVolume(volume);
//	m_pBGMChannelGroup->setVolume(volume);
//}
//
////��ġ����
//void cSoundManager::SetPitch(float pitch)
//{
//	m_pSoundChannelGroup->setPitch(pitch);
//	m_pBGMChannelGroup->setPitch(pitch);
//}
//
////��ġ����
//void cSoundManager::SetPan(float pan)
//{
//	m_pSoundChannelGroup->setPan(pan);
//	m_pBGMChannelGroup->setPan(pan);
//}
