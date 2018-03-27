#pragma once
#include "glm.hpp"
#include <vector>
#include <qstring.h>
#include "OmegaEngineEditor.h"

class LevelManager;

extern LevelManager *p_levelManager;

class LevelManager
{

public:

	struct TextureFileInfo
	{
		QString mainTexFilename;
		QString bgTexFilename;
		QString mainAtlasFilename;
		QString bgAtlasFilename;
	};

	LevelManager() {}
	LevelManager(QString mainAtlasFileName, QString mainTexFileName, QString bgAtlasFileName, QString bgTexFileName, long time, int width, int height);

	bool SaveLevelData(QString filename, std::vector<LineInfo>& collData);
	bool LoadLevelData(QString filename, std::vector<LineInfo>& collData, QWidget *window);

	// helper functions
	void UpdateEntityFilename(QString filename) { m_entityFilename = filename; }
	QString GetEntityFilename() { return m_entityFilename; }

private:

	TextureFileInfo m_fileInfo;
	QString m_entityFilename;

	long m_levelTime;
	int m_worldWidth;
	int m_worldHeight;
};

