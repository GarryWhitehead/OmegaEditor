#pragma once

#include "ui_NewLevel.h"
#include "glm.hpp"

struct LevelData
{
	glm::vec2 worldSize;
	glm::vec2 screenSize;
	long levelTime;
};

class NewLevelWindow : public QWidget
{
	Q_OBJECT

public:
	NewLevelWindow();
	~NewLevelWindow();

private slots:
	
	void CloseNewWindow();
	void OnLoadMainTexture();
	void OnLoadMainAtlas();
	void OnLoadBackgroundTexture();
	void OnLoadBackgroundAtlas();
	void OnPrepareLevel();

private:

	Ui::NewLevelForm *newLevelUi;

	QString mainTexFileName;
	QString mainAtlasFileName;
	QString bgTexFileName;
	QString bgAtlasFileName;
};

