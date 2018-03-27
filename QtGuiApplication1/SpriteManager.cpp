#include "SpriteManager.h"
#include "OmegaEngineEditor.h"
#include "CustomGraphicsView.h"
#include "CustomSpriteView.h"
#include "EntityManager.h"
#include "components/sprite.h"
#include <qmessagebox.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qdebug.h>
#include <qfiledialog.h>
#include <qpixmap.h>
#include <qgraphicsitem.h>
#include <qgraphicsscene.h>
#include <qgraphicsview.h>
#include <string>
#include "utility/file_log.h"

SpriteManager *p_spriteManager = nullptr;

SpriteManager::SpriteManager(QString mainAtlasFileName, QString mainTexFileName, QString bgAtlasFileName, QString bgTexFileName, QWidget *parent)
{
	p_spriteManager = this;

	// load main spreetsheet atlas and image data
	if (!this->PrepareAtlasData(mainAtlasFileName, m_mainAtlasData, m_mainTexW, m_mainTexH)) {
		QMessageBox::critical(parent, "File error", "Unable to open .json file.", QMessageBox::Ok);
		return;
	}

	mainImage = new QImage;
	if (!mainImage->load(mainTexFileName)) {
		QMessageBox::information(parent, "File Error", "Unable to open main sprite sheet file.", QMessageBox::Ok);
		return;
	}
	
	// load background spreetsheet atlas and image data
	if (!this->PrepareAtlasData(bgAtlasFileName, m_bgAtlasData, m_backgroundTexW, m_backgroundTexH)) {
		QMessageBox::critical(parent, "File error", "Unable to open .json file.", QMessageBox::Ok);
		return;
		}

	bgImage = new QImage;
	if (!bgImage->load(bgTexFileName)) {
		QMessageBox::information(parent, "File Error", "Unable to open background sprite sheet file.", QMessageBox::Ok);
	}
	else {
		QMessageBox::information(parent, "Information", "Texture and atlas successfully loaded.", QMessageBox::Ok);
	}
}

SpriteManager::~SpriteManager()
{
}

bool SpriteManager::PrepareAtlasData(QString atlasFileName, std::vector<AtlasData>& atlasData, int& textureW, int& textureH)
{
	// texture atlas data is stored in json file format
	if (atlasFileName.isEmpty()) {
		return false;
	}
	atlasFile = new QFile(atlasFileName);
	if (!atlasFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
		return false;
	}
	QFileInfo fileInfo(atlasFile->fileName());
	atlasPath = fileInfo.path();
	QString data(atlasFile->readAll());
	atlasFile->close();

	QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
	QJsonObject obj = doc.object();
	if (obj.contains("frames") && obj["frames"].isArray()) {
		QJsonArray frameArray = obj["frames"].toArray();
		atlasData.clear();

		// Uv and size data is stored in an array within the "frame" object
		for (auto& val : frameArray) {
			AtlasData atlas;
			QJsonObject frame = val.toObject();	
			atlas.filename = frame["filename"].toString();
			
			QJsonObject texData = frame["frame"].toObject();
			atlas.uv.x = texData["x"].toInt();
			atlas.uv.y = texData["y"].toInt();
			atlas.size.x = texData["w"].toInt();
			atlas.size.y = texData["h"].toInt();
			atlasData.push_back(atlas);
		}
	}

	// also obtain the texture total size if available - stored in the "meta" object
	if (obj.contains("meta")) {									
		QJsonObject meta = obj["meta"].toObject();
		if (meta.contains("size")) {
			QJsonObject size = meta["size"].toObject();
			textureW = size["w"].toInt();
			textureH = size["h"].toInt();
		}
	}
	

	return true;
}

void SpriteManager::ShowSpriteSheet(QGraphicsScene *scene, CustomSpriteView *view, QWidget *window, int spriteType)
{
	if (spriteType == MAIN_SPRITE) {
		item = new QGraphicsPixmapItem(QPixmap::fromImage(*mainImage));
		item->setFlag(QGraphicsItem::ItemIsSelectable);
		scene->addItem(item);
		this->AddRectToImage(scene, m_mainAtlasData);
		view->show();
	}
	else if (spriteType == BACKGROUND_SPRITE) {
		item = new QGraphicsPixmapItem(QPixmap::fromImage(*bgImage));
		item->setFlag(QGraphicsItem::ItemIsSelectable);
		scene->addItem(item);
		this->AddRectToImage(scene, m_bgAtlasData);
		view->show();
	}
}

bool SpriteManager::AddSpriteData(bool state, uint32_t entityID, glm::vec3 pos, uint32_t imageIndex, uint32_t spriteType, uint32_t repeat, float rotation, CustomGraphicsView *mainView, QGraphicsScene *mainScene)
{
	// if the entity is edited, remove the old sprite data
	if (state == EntityManager::EDIT_ENTITY) {
		if(spriteType == MAIN_SPRITE) {
			m_mainSpriteData.erase(std::remove_if(m_mainSpriteData.begin(), m_mainSpriteData.end(),
				[=](const SpriteData data) {return data.entityID == entityID; }), m_mainSpriteData.end());
		}
		else if (spriteType == BACKGROUND_SPRITE) {
			m_bgSpriteData.erase(std::remove_if(m_bgSpriteData.begin(), m_bgSpriteData.end(),
				[=](const SpriteData data) {return data.entityID == entityID; }), m_bgSpriteData.end());
		}
	}

	SpriteData sprite = { entityID, pos, imageIndex, spriteType, repeat, rotation };
	
	if (!RefreshSpriteImage(sprite, mainView, mainScene)) {
		return false;
	}

	if (spriteType == MAIN_SPRITE) {
		m_mainSpriteData.push_back(sprite);
	}
	else if (spriteType == BACKGROUND_SPRITE) {
		m_bgSpriteData.push_back(sprite);
	}
	else {
		return false;
	}
	return true;
}

bool SpriteManager::AddSpriteData(bool state, SpriteData sprite, CustomGraphicsView *mainView, QGraphicsScene *mainScene)
{
	// if the entity is edited, remove the old sprite data
	if (state == EntityManager::EDIT_ENTITY) {
		if (sprite.type == MAIN_SPRITE) {
			m_mainSpriteData.erase(std::remove_if(m_mainSpriteData.begin(), m_mainSpriteData.end(),
				[=](const SpriteData data) {return data.entityID == sprite.entityID; }), m_mainSpriteData.end());
		}
		else if (sprite.type == BACKGROUND_SPRITE) {
			m_bgSpriteData.erase(std::remove_if(m_bgSpriteData.begin(), m_bgSpriteData.end(),
				[=](const SpriteData data) {return data.entityID == sprite.entityID; }), m_bgSpriteData.end());
		}
	}

	if (!RefreshSpriteImage(sprite, mainView, mainScene)) {
		return false;
	}

	if (sprite.type == MAIN_SPRITE) {
		m_mainSpriteData.push_back(sprite);
	}
	else if (sprite.type == BACKGROUND_SPRITE) {
		m_bgSpriteData.push_back(sprite);
	}
	else {
		return false;
	}
	return true;
}

float SpriteManager::RotateSprite(uint32_t ID, int key, uint32_t viewIndex)
{
	QTransform trans;
	if (viewIndex == MAIN_VIEW || viewIndex == VIEW_ALL) {
		for (auto& sprite : m_mainSpriteData) {
			if (sprite.entityID == ID) {

				switch (key) {
				case Qt::Key_A:	sprite.rotation += 1; break;
				case Qt::Key_D: sprite.rotation -= 1; break;
				}

				this->Rotate(sprite);
				return sprite.rotation;
			}
		}
	}
	if (viewIndex == BACKGROUND_VIEW || viewIndex == VIEW_ALL) {
		for (auto& sprite : m_bgSpriteData) {
			if (sprite.entityID == ID) {

				switch (key) {
				case Qt::Key_A:	sprite.rotation += 1; break;
				case Qt::Key_D: sprite.rotation -= 1; break;
				}

				this->Rotate(sprite);
				return sprite.rotation;
			}
		}
	}
	return -1.0f;
}

bool SpriteManager::RefreshSpriteImage(SpriteData& sprite, CustomGraphicsView *mainView, QGraphicsScene *mainScene)
{
	QString filename;
	float spriteWidth;

	if (sprite.type == MAIN_SPRITE) {
		filename = atlasPath + "/" + m_mainAtlasData[sprite.index].filename;
		spriteWidth = m_mainAtlasData[sprite.index].size.x;
	}
	else if (sprite.type == BACKGROUND_SPRITE) {
		filename = atlasPath + "/" + m_bgAtlasData[sprite.index].filename;
		spriteWidth = m_bgAtlasData[sprite.index].size.x;
	}
	else {
		return false;
	}

	sprite.image = new QImage;
	if (!sprite.image->load(filename)) {
		return false;
	}

	sprite.item.resize(sprite.repeatCount);

	for (int c = 0; c < sprite.repeatCount; ++c) {
		sprite.item[c] = new QGraphicsPixmapItem(QPixmap::fromImage(*sprite.image));
		sprite.item[c]->setPos(sprite.pos.x + (c * spriteWidth), sprite.pos.y);
		sprite.item[c]->setFlag(QGraphicsItem::ItemIsMovable);

		if (sprite.rotation != 0) {			// only rotate if value is non-zero
			this->Rotate(sprite);
		}
		mainScene->addItem(sprite.item[c]);
		mainView->show();
	}

	if (sprite.entityID == mainView->currentID()) {
		this->SelectedSprite(sprite.entityID, sprite.type, mainScene);
	}
	return true;
}

void SpriteManager::UpdatePositionCursor(int ID, QPoint pos, QGraphicsScene *scene, CustomGraphicsView *view, int viewIndex)
{
	QPoint viewPos = view->mapFromGlobal(QCursor::pos());
	QPointF scenePos = view->mapToScene(viewPos);
	float spriteWidth = 0.0f;

	if (viewIndex == MAIN_VIEW || viewIndex == VIEW_ALL) {									// only update if the sprite is a main sprite
		for (auto& sprite : m_mainSpriteData) {
			if (sprite.entityID == ID) {
				spriteWidth = m_mainAtlasData[sprite.index].size.x;
				for (int c = 0; c < sprite.repeatCount; ++c) {
					sprite.item[c]->setPos(scenePos.x() + (c * spriteWidth), scenePos.y());
					view->show();
				}
				sprite.pos.x = scenePos.x();
				sprite.pos.y = scenePos.y();
				this->SwitchView(viewIndex, scene, view);
			}
		}
	}
	if (viewIndex == BACKGROUND_VIEW || VIEW_ALL) {						// only update if the sprite is a background sprite
		for (auto& sprite : m_bgSpriteData) {
			if (sprite.entityID == ID) {
				spriteWidth = m_bgAtlasData[sprite.index].size.x;
				for (int c = 0; c < sprite.repeatCount; ++c) {
					sprite.item[c]->setPos(scenePos.x() + (c * spriteWidth), scenePos.y());
					view->show();
				}
				sprite.pos.x = scenePos.x();
				sprite.pos.y = scenePos.y();
				this->SwitchView(viewIndex, scene, view);
			}
		}
	}
}

glm::vec3 SpriteManager::UpdatePositionKeyPress(int ID, int key, float movePos, CustomGraphicsView *view, int viewIndex)
{
	float spriteWidth = 0.0f;

	if (viewIndex == MAIN_VIEW || viewIndex == VIEW_ALL) {									// only update if the sprite is a main sprite		
		for (auto& sprite : m_mainSpriteData) {
			if (sprite.entityID == ID) {
				switch (key) {
				case Qt::Key_Up: sprite.pos.y -= movePos; break;
				case Qt::Key_Down: sprite.pos.y += movePos; break;
				case Qt::Key_Left: sprite.pos.x -= movePos; break;
				case Qt::Key_Right: sprite.pos.x += movePos; break;
				}
				spriteWidth = m_mainAtlasData[sprite.index].size.x;
				for (int c = 0; c < sprite.repeatCount; ++c) {
					sprite.item[c]->setPos(sprite.pos.x + (c * spriteWidth), sprite.pos.y);
					view->show();
				}
				return sprite.pos;
			}
		}
	}
	if (viewIndex == BACKGROUND_VIEW || VIEW_ALL) {						// only update if the sprite is a background sprite
		for (auto& sprite : m_bgSpriteData) {
			if (sprite.entityID == ID) {
				switch (key) {
				case Qt::Key_Up: sprite.pos.y -= movePos; break;
				case Qt::Key_Down: sprite.pos.y += movePos; break;
				case Qt::Key_Left: sprite.pos.x -= movePos; break;
				case Qt::Key_Right: sprite.pos.x += movePos; break;
				}
				spriteWidth = m_bgAtlasData[sprite.index].size.x;
				for (int c = 0; c < sprite.repeatCount; ++c) {
					sprite.item[c]->setPos(sprite.pos.x + (c * spriteWidth), sprite.pos.y);
					view->show();
				}
				return sprite.pos;
			}
		}
	}
}

void SpriteManager::SwitchView(int viewType, QGraphicsScene *scene, CustomGraphicsView *view)
{
	scene->clear();														// completely clear the scene

	if (viewType == BACKGROUND_VIEW || viewType == VIEW_ALL) {
		for (auto& sprite : m_bgSpriteData) {
			this->RefreshSpriteImage(sprite, view, scene);
		}
	}
	if (viewType == MAIN_VIEW || viewType == VIEW_ALL) {
		for (auto& sprite : m_mainSpriteData) {
			this->RefreshSpriteImage(sprite, view, scene);
		}
	}
}

void SpriteManager::DeleteEntitySpriteData(int ID)
{
	// check whether entity is loacted in the main sprite vector
	auto& iterEnd = std::remove_if(m_mainSpriteData.begin(), m_mainSpriteData.end(),
		[=](const SpriteData data) {return data.entityID == ID; });
	if (iterEnd != m_mainSpriteData.end()) {
		m_mainSpriteData.erase(iterEnd, m_mainSpriteData.end());
	}
	else {
		// Otherwise, check background sprite data
		auto& iterEnd = std::remove_if(m_bgSpriteData.begin(), m_bgSpriteData.end(),
			[=](const SpriteData data) {return data.entityID == ID; });
		if (iterEnd != m_bgSpriteData.end()) {
			m_bgSpriteData.erase(iterEnd, m_bgSpriteData.end());
		}
	}
}

// =========================================================================================================================
// Utility functions =======================================================================================================

void SpriteManager::Rotate(SpriteData& sprite)
{
	QTransform trans;
	glm::vec2 size = m_mainAtlasData[sprite.index].size;

	if (sprite.rotation < 0) {
		sprite.rotation = 360;
	}
	else if (sprite.rotation > 360) {
		sprite.rotation = 0;
	}
	trans.translate(size.x / 2, size.y / 2);
	trans.rotate(sprite.rotation);
	trans.translate(-size.x / 2, -size.y / 2);
	sprite.item[0]->setTransform(trans);
}

int SpriteManager::FindIndex(QPoint pos, CustomSpriteView *view, int spriteType)
{
	QPoint viewPos = view->mapFromGlobal(QCursor::pos());
	QPointF scenePos = view->mapToScene(viewPos);
	int index = 0;

	if (spriteType == MAIN_SPRITE) {
		for (auto& atlas : m_mainAtlasData) {
			QRect rect;
			rect.setRect(atlas.uv.x, atlas.uv.y, atlas.size.x, atlas.size.y);
			if (rect.contains(scenePos.x(), scenePos.y())) {
				return index;
			}
			++index;
		}
	}
	else if (spriteType == BACKGROUND_SPRITE) {
		for (auto& atlas : m_bgAtlasData) {
			QRect rect;
			rect.setRect(atlas.uv.x, atlas.uv.y, atlas.size.x, atlas.size.y);
			if (rect.contains(scenePos.x(), scenePos.y())) {
				return index;
			}
			++index;
		}
	}
	return -1;			// inform the caller that the event occured elsewhere
}

int SpriteManager::FindID(QPoint pos, CustomGraphicsView *view, int viewIndex)
{
	QPoint viewPos = view->mapFromGlobal(QCursor::pos());
	QPointF scenePos = view->mapToScene(viewPos);

	if (viewIndex == MAIN_VIEW || viewIndex == VIEW_ALL) {
		for (auto& sprite : m_mainSpriteData) {
			QRect rect;
			glm::vec2 size = m_mainAtlasData[sprite.index].size;
			rect.setRect(sprite.pos.x, sprite.pos.y, size.x * sprite.repeatCount, size.y);
			if (rect.contains(scenePos.x(), scenePos.y())) {
				return sprite.entityID;
			}
		}
	}
	if (viewIndex == BACKGROUND_VIEW || viewIndex == VIEW_ALL) {
		for (auto& sprite : m_bgSpriteData) {
			QRect rect;
			glm::vec2 size = m_bgAtlasData[sprite.index].size;
			rect.setRect(sprite.pos.x, sprite.pos.y, size.x * sprite.repeatCount, size.y);
			if (rect.contains(scenePos.x(), scenePos.y())) {
				return sprite.entityID;
			}
		}
	}
	return -1;			// inform the caller that the event occured elsewhere
}

void SpriteManager::AddRectToImage(QGraphicsScene *scene, std::vector<AtlasData>& atlasData)
{
	m_spriteRect.resize(atlasData.size());
	int index = 0;

	for (auto& sprite : atlasData) {
		QGraphicsRectItem *rectItem;
		rectItem = scene->addRect(sprite.uv.x, sprite.uv.y, sprite.size.x, sprite.size.y);

		// also add sprite frame number to rect
		QGraphicsTextItem *textItem;
		textItem = scene->addText(QString::number(index++));
		textItem->setPos(sprite.uv.x + (sprite.size.x / 2), sprite.uv.y + (sprite.size.y / 2));
		m_spriteRect.push_back(rectItem);
		m_spriteText.push_back(textItem);
	}
}

void SpriteManager::SelectedSprite(uint32_t ID, uint32_t spriteType, QGraphicsScene *scene)
{
	if (spriteType == MAIN_SPRITE) {
		for (auto& sprite : m_mainSpriteData) {
			if (sprite.entityID == ID) {
				AtlasData atlas = m_mainAtlasData[sprite.index];
				scene->addRect(sprite.pos.x, sprite.pos.y, atlas.size.x * sprite.repeatCount, atlas.size.y);
			}
		}
	}
	if (spriteType == BACKGROUND_SPRITE) {
		for (auto& sprite : m_bgSpriteData) {
			if (sprite.entityID == ID) {
				AtlasData atlas = m_bgAtlasData[sprite.index];
				scene->addRect(sprite.pos.x, sprite.pos.y, atlas.size.x * sprite.repeatCount, atlas.size.y);
			}
		}
	}
}

int SpriteManager::GetTextureWidth(AtlasID atlasId)
{
	if (atlasId == AtlasID::MAIN_ATLAS) {
		return m_mainTexW;
	}
	return m_backgroundTexW;
}


int SpriteManager::GetTextureHeight(AtlasID atlasId)
{
	if (atlasId == AtlasID::MAIN_ATLAS) {
		return m_mainTexH;
	}
	return m_backgroundTexH;
}