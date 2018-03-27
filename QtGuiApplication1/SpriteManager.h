#pragma once
#include <qfile.h>
#include "ui_OmegaEngineEditor.h"
#include "glm.hpp"
#include <vector>
#include <string>
#include <qgraphicsitem.h>

class SpriteManager;
class CustomGraphicsView;
class CustomSpriteView;

extern SpriteManager *p_spriteManager;

class SpriteManager
{
public:

	enum class AtlasID
	{
		MAIN_ATLAS,
		BACKGROUND_ATLAS
	};

	struct AtlasData
	{
		QString filename;
		glm::vec2 uv;
		glm::vec2 size;
	};

	struct SpriteData
	{
		SpriteData() :
			entityID(0),
			index(0),
			repeatCount(1),
			image(nullptr)
		{}

		SpriteData(uint32_t ID, glm::vec3 p, uint32_t i, uint32_t t, uint32_t r, float rot) :
			entityID(ID),
			pos(p),
			index(i),
			type(t),
			repeatCount(r),
			rotation(rot),
			image(nullptr)
		{}

		uint32_t entityID;
		glm::vec3 pos;
		uint32_t index;
		uint32_t type;
		uint32_t repeatCount;
		float rotation;
		float scale;
		QImage *image;
		std::vector<QGraphicsPixmapItem*> item;
	};


	SpriteManager(QString mainAtlasFileName, QString mainTexFileName, QString bgAtlasFileName, QString bgTexFileName, QWidget *parent);

	~SpriteManager();

	bool PrepareAtlasData(QString atlasFileName, std::vector<AtlasData>& atlasData, int& textureW, int& textureH);
	void ShowSpriteSheet(QGraphicsScene *scene, CustomSpriteView *view, QWidget *window, int spriteType);
	float RotateSprite(uint32_t ID, int key, uint32_t spriteType);
	bool AddSpriteData(bool state, uint32_t entityID, glm::vec3 pos, uint32_t imageIndex, uint32_t spriteType, uint32_t repeat, float rotation, CustomGraphicsView *mainView, QGraphicsScene *mainScene);
	bool AddSpriteData(bool state, SpriteData sprite, CustomGraphicsView *mainView, QGraphicsScene *mainScene);
	bool RefreshSpriteImage(SpriteData& sprite, CustomGraphicsView *mainView, QGraphicsScene *mainScene);
	void UpdatePositionCursor(int ID, QPoint pos, QGraphicsScene *scene, CustomGraphicsView *view, int viewIndex);
	glm::vec3 UpdatePositionKeyPress(int ID, int key, float movePos, CustomGraphicsView *view, int viewIndex);
	void SwitchView(int viewType, QGraphicsScene *scene, CustomGraphicsView *view);
	void DeleteEntitySpriteData(int ID);

	// utility functions
	void Rotate(SpriteData& sprite);
	int FindID(QPoint pos, CustomGraphicsView *view, int viewIndex);
	int FindIndex(QPoint pos, CustomSpriteView *view, int spriteType);
	void AddRectToImage(QGraphicsScene *scene, std::vector<AtlasData>& atlasData);
	void SelectedSprite(uint32_t ID, uint32_t imageView, QGraphicsScene *scene);

	// helper functions
	AtlasData GetMainAtlasData(const int index) { return m_mainAtlasData[index]; }
	AtlasData GetBackgroundAtlasData(const int index) { return m_bgAtlasData[index]; }
	uint32_t GetMainAtlasDataSize() const { return m_mainAtlasData.size(); }
	uint32_t GetBackgroundAtlasDataSize() const { return m_bgAtlasData.size(); }
	int GetTextureWidth(AtlasID atlasId); 
	int GetTextureHeight(AtlasID atlasID); 

private:

	// data for the texture sprite-sheet
	QFile *atlasFile;
	QString atlasPath;
	QImage *mainImage;
	QImage *bgImage;
	QGraphicsPixmapItem *item;
	std::vector<QGraphicsRectItem*> m_spriteRect;
	std::vector<QGraphicsTextItem*> m_spriteText;

	int m_backgroundTexW, m_backgroundTexH;
	int m_mainTexW, m_mainTexH;

	// individual sprite data
	std::vector<AtlasData> m_mainAtlasData;
	std::vector<AtlasData> m_bgAtlasData;
	std::vector<SpriteData> m_mainSpriteData;
	std::vector<SpriteData> m_bgSpriteData;
};

