#include "level_manager.h"
#include "OmegaEngineEditor.h"
#include "CustomGraphicsView.h"
#include "CustomSpriteView.h"
#include "SpriteManager.h"
#include "components/sprite.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qfiledialog.h>

LevelManager *p_levelManager = nullptr;

LevelManager::LevelManager(QString mainAtlasFileName, QString mainTexFileName, QString bgAtlasFileName, QString bgTexFileName, long time, int width, int height)
{
	p_levelManager = this;

	m_fileInfo.bgAtlasFilename = bgAtlasFileName;
	m_fileInfo.bgTexFilename = bgTexFileName;
	m_fileInfo.mainAtlasFilename = mainAtlasFileName;
	m_fileInfo.mainTexFilename = mainTexFileName;

	m_levelTime = time;
	m_worldWidth = width;
	m_worldHeight = height;
}

bool LevelManager::SaveLevelData(QString filename, std::vector<LineInfo>& collData)
{
	QJsonObject levelObj;
	// add texture and atlas filenames to json doc
	levelObj["Background Texture"] = QString(m_fileInfo.bgTexFilename);
	levelObj["Main Texture"] = QString(m_fileInfo.mainTexFilename);
	levelObj["Background Atlas"] = QString(m_fileInfo.bgAtlasFilename);
	levelObj["Main Atlas"] = QString(m_fileInfo.mainAtlasFilename);

	// entity data path
	levelObj["Entity Data"] = QString(m_entityFilename);

	// level variables
	levelObj["Level Time"] = m_levelTime;
	levelObj["World Width"] = m_worldWidth;
	levelObj["World Height"] = m_worldHeight;

	// json array containing collision mask data with y-axis flipped for vulkan compatibility
	if (!collData.empty()) {
		QJsonArray collMaskArray;
		for (auto& line : collData) {
			QJsonObject lineObj;
			lineObj["end X"] = line.end.x;
			lineObj["end Y"] = m_worldHeight - line.end.y;
			lineObj["start X"] = line.start.x;
			lineObj["start Y"] = m_worldHeight - line.start.y;
			collMaskArray.append(lineObj);
		}
		levelObj["Collision Masks"] = collMaskArray;
	}

	QJsonDocument doc(levelObj);
	QFile *levelFile = new QFile(filename);
	if (!levelFile->open(QIODevice::WriteOnly | QIODevice::Text)) {
		return false;
	}
	levelFile->write(doc.toJson());

	levelFile->close();
	delete levelFile;

	return true;
}

bool LevelManager::LoadLevelData(QString filename, std::vector<LineInfo>& collData, QWidget *window)
{
	if (filename.isEmpty()) {
		return false;
	}
	QFile *levelFile = new QFile(filename);
	if (!levelFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
		return false;
	}
	QFileInfo fileInfo(levelFile->fileName());
	QString data(levelFile->readAll());

	QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
	QJsonObject obj = doc.object();

	// get filenames
	m_entityFilename = obj["Entity Data"].toString();

	m_fileInfo.bgAtlasFilename = obj["Background Atlas"].toString();
	m_fileInfo.bgTexFilename = obj["Background Texture"].toString();
	m_fileInfo.mainAtlasFilename = obj["Main Atlas"].toString();
	m_fileInfo.mainTexFilename = obj["Main Texture"].toString();
	
	// level variables
	m_levelTime = obj["Level Time"].toInt();
	m_worldWidth = obj["World Width"].toInt();
	m_worldHeight = obj["World Height"].toInt();

	// collision points
	QJsonArray pointsArray = obj["Collision Masks"].toArray();
	for (auto& point : pointsArray) {
		LineInfo line;
		QJsonObject lineObj = point.toObject();
		line.end.x = lineObj["end X"].toInt();
		line.end.y = lineObj["end Y"].toInt(); ;
		line.start.x = lineObj["start X"].toInt();
		line.start.y = lineObj["start Y"].toInt();
		line.end.y = m_worldHeight - line.end.y;
		line.start.y = m_worldHeight - line.start.y;
		collData.push_back(line);
	}
	
	// cleanup now we have the data
	levelFile->close();
	delete levelFile;

	// delete the current sprite manager and create a new one with the new level data
	if (p_spriteManager != nullptr) {
		delete p_spriteManager;
	}

	p_spriteManager = new SpriteManager(m_fileInfo.mainAtlasFilename, m_fileInfo.mainTexFilename, m_fileInfo.bgAtlasFilename, m_fileInfo.bgTexFilename, window);

	return true;
}