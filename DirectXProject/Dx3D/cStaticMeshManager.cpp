#include "stdafx.h"
#include "cStaticMeshManager.h"
#include "cStaticMesh.h"


cStaticMeshManager::cStaticMeshManager()
{
}


cStaticMeshManager::~cStaticMeshManager()
{
}

cStaticMesh * cStaticMeshManager::GetStaticMesh(char * szFolder, char * szFilename)
{
	std::string sFullPath(szFolder);
	sFullPath += std::string(szFilename);

	if (m_mapStaticMesh.find(sFullPath) == m_mapStaticMesh.end())
	{
		cStaticMesh* pStaticMesh = new cStaticMesh();
		pStaticMesh->Setup(szFolder, szFilename);
		m_mapStaticMesh[sFullPath] = pStaticMesh;
	}
	return m_mapStaticMesh[sFullPath];
}

void cStaticMeshManager::Destory()
{
	for each(auto it in m_mapStaticMesh)
	{
		it.second->Destroy();
		SAFE_DELETE(it.second);
	}
}
