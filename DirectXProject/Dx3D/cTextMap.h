#pragma once
class cBuilding;
class cFrustum;

class cTextMap : public iMap
{
	std::vector<cBuilding*>		m_vpBuildings;
	cFrustum*					m_pFrustum;

public:
	cTextMap();
	~cTextMap();

	void Setup(IN char* szFilename, IN D3DXMATRIXA16* pmat = NULL);
	void Update();
	void Render();

	// iMap override
	virtual bool GetHeight(IN float x, OUT float& y, IN float z) override;

	std::vector<cBuilding*> GetBuildings() { return m_vpBuildings; }
};

