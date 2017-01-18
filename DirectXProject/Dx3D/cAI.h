#pragma once
//#include "cCharacter.h"
//
//class cGun;
//class cController;
//
//class cAI : public cCharacter
//{
//	cGun*				m_pGun;
//
//public:
//	cAI();
//	virtual ~cAI();
//
//	virtual void Setup(char* szFolder, char* szFilename) override;
//	virtual void Update(iMap* pMap) override;
//	virtual void Render() override;
//};
//

class cGun;
class cController;
//class cOBB;

class cAI : public cGameObject
{
	cGun*				m_pGun;
	cController*		m_pController;

	cSkinnedMesh*		m_pSkinnedMesh;				// ĳ���� ��ü
	//cOBB*				m_pOBB;						// OBB Box

	iMap*				m_pMap;						// iMap

public:
	cAI();
	~cAI();

	void Setup(char* szFolder, char* szFilename);
	void Update(iMap* pMap);
	void Render();

	//cOBB* GetOBB() { return m_pOBB; }
};

