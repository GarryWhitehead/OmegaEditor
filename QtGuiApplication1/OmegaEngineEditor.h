#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OmegaEngineEditor.h"
#include <qtimer.h>
#include "glm.hpp"

class NewEntityWin;
class ListEntityWin;
class NewLevelWindow;
class CustomGraphicsView;

const int MAIN_VIEW = 0;
const int BACKGROUND_VIEW = 1;
const int VIEW_ALL = 2;

struct LineInfo
{
	glm::vec2 start;
	glm::vec2 end;
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:

	static const int GRAPHIC_VIEW_WIDTH = 1280;
	static const int GRAPHIC_VIEW_HEIGHT = 768;
	static const int WORLD_WIDTH = 3840;
	static const int WORLD_HEIGHT = 2304;
	
	enum class CoordSystem
	{
		BOTTOM_LEFT,
		TOP_LEFT
	};

	MainWindow(QWidget *parent = Q_NULLPTR);

	~MainWindow();

	bool CheckForSave();
	void CursorUpdateSpritePosition(int ID, QPoint pos);
	void KeyUpdateSpritePosition(int ID, int key, float movePos);
	void KeyRotateSprite(uint32_t ID, int key);
	int FindEntityID(QPoint pos);
	void UpdateSelectedEntity(uint32_t ID);
	void ShowEntityData(uint32_t ID);
	void PasteFromClipboard(int32_t id);
	void DeleteEntity(uint32_t ID);
	void SetIconStatus(bool status);

	// collision mask functions
	void SetCollisionMask(QPoint pos);
	void RefreshLineView();
	void TerminateCollMask();
	void DeleteCollisionMask();

	// helper functions
	QString GetEntityFilename() { return m_entityFileName; }

private slots:
	void OnFileOpen();
	void OnFileSave();
	void OnFileNew();
	void OnLevelOpen();
	void OnLevelSave();
	void OpenAddEntity();
	void OpenDeleteEntity();
	void OpenListEntity();
	void OpenCopyEntity();
	void OpenPasteEntity();
	void OpenCollisionMask();
	void comboBoxChange(const int index);
	
signals:
	void currentIndexChanged(const int index);

protected:
	
	
private:
	Ui::OmegaEngineEditorClass *ui;
	NewEntityWin *newEntityWin;
	ListEntityWin *listEntityWin;
	NewLevelWindow *newLevelWin;
	CustomGraphicsView *customView;
	QComboBox *comboBox;
	QGraphicsScene *scene;
	QAction *m_collMaskAction;
	bool m_moveSprite;
	
	uint32_t m_dragIndex;
	QString m_entityFileName;
	bool m_dataLoaded;

	std::vector<LineInfo> m_collMaskPoints;
	std::vector<LineInfo> m_currentCollMask;

	// Action variables - File menu
	QAction *m_newAction;
	QAction *m_openAction;
	QAction *m_saveAction;
	QAction *m_openFileAction;
	QAction *m_saveFileAction;
	QAction *m_exitAction;

	//Edit menu
	QAction *m_copyAction;
	QAction *m_pasteAction;
	
	// Entity menu
	QAction *m_newEntAction;
	QAction *m_delEntAction;
	QAction *m_listEntAction;
};
