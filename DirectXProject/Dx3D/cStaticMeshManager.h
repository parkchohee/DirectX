#pragma once

#define g_pStaticMeshManager cStaticMeshManager::GetInstance()

class cStaticMesh;

class cStaticMeshManager
{
private:
	std::map<std::string, cStaticMesh*> m_mapStaticMesh;

public:
	SINGLETONE(cStaticMeshManager);

	cStaticMesh* GetStaticMesh(char* szFolder, char* szFilename);
	
	void Destroy();
};

