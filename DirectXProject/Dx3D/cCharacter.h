#pragma once

class iMap;
class cSkinnedMesh;
class cOBB;
class cController;

class cCharacter : public cGameObject
{
protected:
	cSkinnedMesh*		m_pSkinnedMesh;				// ĳ���� ��ü
	cOBB*				m_pOBB;						// OBB Box
	cController*		m_pController;		// ��Ʈ�ѷ�

	D3DXMATRIXA16		m_matWorld;					// worldMatrix
	iMap*				m_pMap;						// iMap

	SYNTHESIZE(float, m_fCurrentHp, CurrentHp);
	
public:
	cCharacter(void);
	virtual ~cCharacter(void);

	virtual void Setup(char* szFolder, char* szFilename);
	virtual void Update(iMap* pMap);
	virtual void Render();
	virtual D3DXMATRIXA16* GetTransform()
	{
		return &m_matWorld;
	}
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

