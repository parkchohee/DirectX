#pragma once
class cStaticMesh;
class cOBB;

class cBuilding : public cObject
{
	cStaticMesh*			m_pBuilding;
	std::string			m_sFolderName;
	std::string			m_sFileName;
	cOBB*			m_pOBB;	/// >> : OBB

	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE(float, m_fAngle, Angle);
	SYNTHESIZE(float, m_fScale, Scale);

public:
	cBuilding(char* szFolder, char* szFilename);
	~cBuilding();

	void Setup(char* szFolder, char* szFilename);
	void Update();
	void Render();

	std::string GetFolderName() { return m_sFolderName; }
	std::string GetFileName() { return m_sFileName; }
};

