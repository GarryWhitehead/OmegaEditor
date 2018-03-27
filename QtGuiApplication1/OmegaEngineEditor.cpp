#include "OmegaEngineEditor.h"
#include "ui_OmegaEngineEditor.h"
#include "EntityManager.h"
#include "SpriteManager.h"
#include "level_manager.h"
#include "NewEntityWin.h"
#include "ListEntityWin.h"
#include "NewLevelWindow.h"
#include "CustomGraphicsView.h"
#include "utility/file_log.h"
#include <qmenubar.h>
#include <qpushbutton.h>
#include <qstatusbar.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qcombobox.h>
#include <qevent.h>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	newEntityWin(nullptr),
	listEntityWin(nullptr),
	newLevelWin(nullptr),
	m_dataLoaded(false),
	m_moveSprite(false)
{	
	m_collMaskPoints.clear();

	// sort out the main window by setting up the widgets
	ui = new Ui::OmegaEngineEditorClass();
	ui->setupUi(this);

	// initilaise custom graphics view to custom mouse events for sprite movement 
	customView = new CustomGraphicsView(this, this);
	customView->setGeometry(5, 60, GRAPHIC_VIEW_WIDTH, GRAPHIC_VIEW_HEIGHT);
	customView->setSceneRect(0, 0, WORLD_WIDTH, WORLD_HEIGHT);

	comboBox = new QComboBox(this);
	comboBox->setGeometry(600, GRAPHIC_VIEW_HEIGHT + 65, 141, 22);
	comboBox->insertItem(0, QString("Main"));
	comboBox->insertItem(1, QString("Background"));
	comboBox->insertItem(2, QString("All"));
	connect(comboBox, SIGNAL(activated(int)), this, SLOT(comboBoxChange(int)));

	// Do not allow resizing of the window
	this->setFixedSize(QSize(1310, 870));
	customView->setDragMode(QGraphicsView::NoDrag);
	customView->setInteractive(true);
	customView->setMouseTracking(true);
	
	// and initiliase the scene required for the graphics view
	scene = new QGraphicsScene();
	customView->setScene(scene);

	// Add a menu bar to the main window with File, Entity menus
	QMenu *fileMenu = ui->menuBar->addMenu("&File");
	
	// new level
	const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
	m_newAction = new QAction(newIcon, "&New Level", this);
	m_newAction->setShortcuts(QKeySequence::New);
	connect(m_newAction, &QAction::triggered, this, &MainWindow::OnFileNew);
	fileMenu->addAction(m_newAction);
	fileMenu->addSeparator();
	ui->mainToolBar->addAction(m_newAction);
	
	// open level
	m_openAction = new QAction("&Open Level", this);
	connect(m_openAction, &QAction::triggered, this, &MainWindow::OnLevelOpen);
	fileMenu->addAction(m_openAction);

	// save level
	m_saveAction = new QAction("&Save Level", this);
	connect(m_saveAction, &QAction::triggered, this, &MainWindow::OnLevelSave);
	fileMenu->addAction(m_saveAction);
	m_saveAction->setEnabled(false);

	fileMenu->addSeparator();

	// open file action
	const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
	m_openFileAction = new QAction(openIcon, "&Open Data", this);
	m_openFileAction->setShortcuts(QKeySequence::Open);
	connect(m_openFileAction, &QAction::triggered, this, &MainWindow::OnFileOpen);
	fileMenu->addAction(m_openFileAction);
	ui->mainToolBar->addAction(m_openFileAction);
	m_openFileAction->setEnabled(false);

	// save file action
	const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
	m_saveFileAction = new QAction(saveIcon, "&Save Data", this);
	m_saveFileAction->setShortcuts(QKeySequence::Save);
	connect(m_saveFileAction, &QAction::triggered, this, &MainWindow::OnFileSave);
	fileMenu->addAction(m_saveFileAction);
	ui->mainToolBar->addAction(m_saveFileAction);
	m_saveFileAction->setEnabled(false);

	// exit application action
	m_exitAction = new QAction("&Exit", this);
	connect(m_exitAction, &QAction::triggered, this, &MainWindow::close);
	fileMenu->addAction(m_exitAction);

	// Edit menu ====================================================================================================
	QMenu *editMenu = ui->menuBar->addMenu("&Edit");

	// copy entity 
	m_copyAction = new QAction("&Copy", this);
	connect(m_copyAction, &QAction::triggered, this, &MainWindow::OpenCopyEntity);
	editMenu->addAction(m_copyAction);
	m_copyAction->setEnabled(false);

	m_pasteAction = new QAction("&Paste", this);
	connect(m_pasteAction, &QAction::triggered, this, &MainWindow::OpenPasteEntity);
	editMenu->addAction(m_pasteAction);
	m_pasteAction->setEnabled(false);

	// Entity menu ==================================================================================================
	QMenu *entityMenu = ui->menuBar->addMenu("&Entity");

	// add entity action
	const QIcon newEntIcon = QIcon::fromTheme("new_entity", QIcon(":/images/new_ent.png"));
	m_newEntAction = new QAction(newEntIcon, "&Add", this);
	connect(m_newEntAction, &QAction::triggered, this, &MainWindow::OpenAddEntity);
	entityMenu->addAction(m_newEntAction);
	ui->mainToolBar->addAction(m_newEntAction);
	m_newEntAction->setEnabled(false);

	// add entity action
	const QIcon delEntIcon = QIcon::fromTheme("del_entity", QIcon(":/images/del_ent.png"));
	m_delEntAction = new QAction(delEntIcon, "&Delete", this);
	connect(m_delEntAction, &QAction::triggered, this, &MainWindow::OpenDeleteEntity);
	entityMenu->addAction(m_delEntAction);
	ui->mainToolBar->addAction(m_delEntAction);
	m_delEntAction->setEnabled(false);

	// List entities action
	const QIcon listEntIcon = QIcon::fromTheme("list_entity", QIcon(":/images/list_ent.png"));
	m_listEntAction = new QAction(listEntIcon, "&List", this);
	connect(m_listEntAction, &QAction::triggered, this, &MainWindow::OpenListEntity);
	entityMenu->addAction(m_listEntAction);
	ui->mainToolBar->addAction(m_listEntAction);
	m_listEntAction->setEnabled(false);

	// Utility menu ==================================================================================================
	QMenu *utilityMenu = ui->menuBar->addMenu("&Utility");
	
	// add collision vectors
	m_collMaskAction = new QAction("&Collision Mask", this);
	m_collMaskAction->setCheckable(true);
	m_collMaskAction->setChecked(false);
	connect(m_collMaskAction, &QAction::triggered, this, &MainWindow::OpenCollisionMask);
	utilityMenu->addAction(m_collMaskAction);
	m_collMaskAction->setEnabled(false);
}

MainWindow::~MainWindow()
{
	if (newLevelWin != nullptr) {
		delete newLevelWin;
	}

	if (newEntityWin != nullptr) {
		delete newEntityWin;
	}
	delete comboBox;
	delete scene;
	delete customView;
	delete ui;
}

void MainWindow::OnFileOpen()
{
	SaveMode saveMode;

	QFileDialog fileDialog(this, tr("Load Entity Data"), "", tr("Binary (*.bin);;Text (*.txt);;All Files (*)"));
	if (fileDialog.exec()) {
		m_entityFileName = fileDialog.selectedFiles()[0];
		if (fileDialog.selectedNameFilter() == QString("Text (*.txt)")) {
			saveMode = SaveMode::SAVE_TEXT;
		}
		else if (fileDialog.selectedNameFilter() == QString("Binary (*.bin)")) {
			saveMode = SaveMode::SAVE_BINARY;
		}
		else {
			QMessageBox msgBox;
			msgBox.setText(QString("No file type stated."));
			msgBox.exec();
			return;
		}

		if (!entityManager->FileSerialisedData(entityManager, m_entityFileName.toStdString(), saveMode, std::fstream::in, FileMode::FILE_IN)) {
			QMessageBox msgBox;
			msgBox.setText(QString("Unable to open %1").arg(m_entityFileName));
			msgBox.exec();
			return;
		}

		// In Qt 0,0 equates to the top left corner whilst in Vulkan, the bottom left is 0,0. So convert before saving/loading
		entityManager->ConvertCoordsToVulkanQt(CoordSystem::TOP_LEFT);
		entityManager->ConvertUvToVulkanQt(CoordSystem::TOP_LEFT);

		// update filename with level manager as this will be saved with the level data
		p_levelManager->UpdateEntityFilename(m_entityFileName);

		if (!entityManager->UpdateAllSpriteData(customView, scene)) {
			QMessageBox msgBox;
			msgBox.setText(QString("Error initialising sprite data"));
			msgBox.exec();
		}
		else {
			ui->statusBar->showMessage(QString("%1 successfully loaded.").arg(m_entityFileName), 3000);
		}
	}
}

void MainWindow::OnFileSave()
{
	SaveMode saveMode;

	QFileDialog fileDialog(this, tr("Save Entity Data"), "", tr("Binary (*.bin);;Text (*.txt);;All Files (*)"));
	if (fileDialog.exec()) {
		m_entityFileName = fileDialog.selectedFiles()[0];
		if (fileDialog.selectedNameFilter() == QString("Text (*.txt)")) {
			saveMode = SaveMode::SAVE_TEXT;
		}
		else if (fileDialog.selectedNameFilter() == QString("Binary (*.bin)")) {
			saveMode = SaveMode::SAVE_BINARY;
		}
		else {
			QMessageBox msgBox;
			msgBox.setText(QString("No file type stated."));
			msgBox.exec();
			return;
		}

		// In Qt 0,0 equates to the top left corner whilst in Vulkan, the bottom left is 0,0. So convert before saving/loading
		// requires dimensions - (total image height - y pos) - sprite height 
		entityManager->ConvertCoordsToVulkanQt(CoordSystem::BOTTOM_LEFT);
		entityManager->ConvertUvToVulkanQt(CoordSystem::BOTTOM_LEFT);

		// update filename with level manager as this will be saved with the level data
		p_levelManager->UpdateEntityFilename(m_entityFileName);

		if (!entityManager->FileSerialisedData(entityManager, m_entityFileName.toStdString(), saveMode, std::fstream::out, FileMode::FILE_OUT)) {
			QMessageBox msgBox;
			msgBox.setText(QString("Unable to open %1").arg(m_entityFileName));
			msgBox.exec();
		}
		else {
			ui->statusBar->showMessage(QString("%1 successfully saved.").arg(m_entityFileName), 3000);
		}
	}
}

void MainWindow::OnFileNew()
{
	newLevelWin = new NewLevelWindow();
}

void MainWindow::OnLevelOpen()
{
	QString fileName;
	SaveMode saveMode;

	if (p_levelManager != nullptr) {
		delete p_levelManager;
	}
	p_levelManager = new LevelManager();

	QFileDialog fileDialog(this, tr("Load Level Data"), "", tr("Level (*.lvl);;All Files (*)"));
	if (fileDialog.exec()) {
		fileName = fileDialog.selectedFiles()[0];
	
		if (!p_levelManager->LoadLevelData(fileName, m_collMaskPoints, this)) {
			QMessageBox msgBox;
			msgBox.setText(QString("Unable to save %1").arg(fileName));
			msgBox.exec();
		}

		// find the suffix to determine whether saved as text or bin
		auto &iter = std::find(fileName.toStdString().begin(), fileName.toStdString().end(), '.bin');
		if(iter != fileName.toStdString().end()) {
			saveMode = SaveMode::SAVE_BINARY;
		}
		else {
			iter = std::find(fileName.toStdString().begin(), fileName.toStdString().end(), '.txt');
			if (iter != fileName.toStdString().end()) {
				saveMode = SaveMode::SAVE_TEXT;
			}
			else {
				QMessageBox msgBox;
				msgBox.setText(QString("Invalid file type."));
				msgBox.exec();
				return;
			}
		}

		// update the entity data filename before loading data
		m_entityFileName = p_levelManager->GetEntityFilename();

		// load serilasied data into map
		if (!entityManager->FileSerialisedData(entityManager, m_entityFileName.toStdString(), saveMode, std::fstream::in, FileMode::FILE_IN)) {
			QMessageBox msgBox;
			msgBox.setText(QString("Unable to open %1").arg(m_entityFileName));
			msgBox.exec();
			return;
		}

		// In Qt 0,0 equates to the top left corner whilst in Vulkan, the bottom left is 0,0. So convert before saving/loading
		entityManager->ConvertCoordsToVulkanQt(CoordSystem::TOP_LEFT);
		entityManager->ConvertUvToVulkanQt(CoordSystem::TOP_LEFT);

		// initialise sprite data and show on screen
		if (!entityManager->UpdateAllSpriteData(customView, scene)) {
			QMessageBox msgBox;
			msgBox.setText(QString("Error initialising sprite data"));
			msgBox.exec();
		}
		else {
			ui->statusBar->showMessage(QString("Level successfully loaded and initialised."), 3000);
		}

		// enable menu icons
		this->SetIconStatus(true);
		m_dataLoaded = true;

	}
}

void MainWindow::OnLevelSave()
{
	QString fileName;
	SaveMode saveMode;

	if (p_levelManager == nullptr) {
		return;
	}

	QFileDialog fileDialog(this, tr("Save Entity Data"), "", tr("Level (*.lvl);;All Files (*)"));
	if (fileDialog.exec()) {
		fileName = fileDialog.selectedFiles()[0];

		if (!p_levelManager->SaveLevelData(fileName, m_collMaskPoints)) {
			QMessageBox msgBox;
			msgBox.setText(QString("Unable to save %1").arg(fileName));
			msgBox.exec();
		}

	}

}

void MainWindow::OpenAddEntity()
{

	newEntityWin = new NewEntityWin(-1, customView, scene);							// passing -1 indicates that we want to create a new entity instance

	uint32_t entityCount = entityManager->GetEntityCount();
	ui->statusBar->showMessage(QString("Entity Created. Entity Count = %1.").arg(entityCount), 3000);
}

void MainWindow::OpenDeleteEntity()
{
	this->DeleteEntity(customView->currentID());
}

void MainWindow::comboBoxChange(const int index)
{
	int viewType = comboBox->currentIndex();
	p_spriteManager->SwitchView(viewType, scene, customView);
}

void MainWindow::OpenListEntity()
{
	listEntityWin = new ListEntityWin();
}

void MainWindow::OpenCopyEntity()
{

}

void MainWindow::OpenPasteEntity()
{

}

void MainWindow::OpenCollisionMask() 
{
	if (!m_collMaskAction->isChecked()) {
		customView->SetCollisionMask(false);
		ui->statusBar->showMessage(QString("Colliion Mask Saved"), 3000);
	}

	customView->SetCollisionMask(true);
	ui->statusBar->showMessage(QString("Colliion Mask Editing Enabled"), 3000);
}

void MainWindow::SetCollisionMask(QPoint pos)
{
	QPoint viewPos = customView->mapFromGlobal(QCursor::pos());
	QPointF scenePos = customView->mapToScene(viewPos);

	LineInfo line;
	if (m_currentCollMask.empty()) {

		if (m_collMaskPoints.empty()) {
			line.start = glm::vec2(scenePos.x(), scenePos.y());
			m_currentCollMask.push_back(line);

			ui->statusBar->showMessage(QString("Start Point added at position x %1, y %2").arg(scenePos.x()).arg(scenePos.y()), 3000);
		}
		else {
			LineInfo lastLine = m_collMaskPoints.back();
			line.start = lastLine.end;
			line.end = glm::vec2(scenePos.x(), scenePos.y());

			m_collMaskPoints.push_back(line);
			m_currentCollMask.clear();

			scene->addLine(line.start.x, line.start.y, line.end.x, line.end.y);
			scene->update();

			ui->statusBar->showMessage(QString("End Point added at position x %1, y %2").arg(line.end.x).arg(line.end.y), 3000);
		}
	}
	else {
		line = m_currentCollMask[0];
		line.end = glm::vec2(scenePos.x(), scenePos.y());

		m_collMaskPoints.push_back(line);
		m_currentCollMask.clear();

		scene->addLine(line.start.x, line.start.y, line.end.x, line.end.y);
		scene->update();

		ui->statusBar->showMessage(QString("End Point added at position x %1, y %2").arg(scenePos.x()).arg(scenePos.y()), 3000);
	}
}

void MainWindow::RefreshLineView()
{
	for (auto& line : m_collMaskPoints) {

		scene->addLine(line.start.x, line.start.y, line.end.x, line.end.y);
	}
	scene->update();
}

void MainWindow::TerminateCollMask()
{
	m_collMaskAction->setChecked(false);
	m_currentCollMask.clear();

	if (!m_collMaskPoints.empty()) {
		ui->statusBar->showMessage(QString("Colliion Mask Saved."), 3000);
	}
	else {
		ui->statusBar->showMessage(QString("Colliion Mask Editing Disabled."), 3000);
	}

	p_spriteManager->SwitchView(comboBox->currentIndex(), scene, customView);
}

void MainWindow::DeleteCollisionMask()
{
	m_currentCollMask.clear();
	m_collMaskPoints.clear();

	p_spriteManager->SwitchView(comboBox->currentIndex(), scene, customView);
	ui->statusBar->showMessage(QString("Colliion Mask Deleted."), 3000);
}

int MainWindow::FindEntityID(QPoint pos)
{
	int index = p_spriteManager->FindID(pos, customView, comboBox->currentIndex());
	return index;
}

void MainWindow::UpdateSelectedEntity(uint32_t ID)
{
	p_spriteManager->SwitchView(comboBox->currentIndex(), scene, customView);
}

void MainWindow::CursorUpdateSpritePosition(int ID, QPoint pos)
{
	QPoint viewPos = customView->mapFromGlobal(QCursor::pos());
	QPointF scenePos = customView->mapToScene(viewPos);

	if (ID >= 0) {
		p_spriteManager->UpdatePositionCursor(ID, pos, scene, customView, comboBox->currentIndex());
		entityManager->UpdateSpriteComponent(ID, scenePos);

		glm::vec2 convertPos = entityManager->ConvertCoords(glm::vec3(scenePos.x(), scenePos.y(), 0.0f), ID);
		ui->statusBar->showMessage(QString("Entity ID: %1. Position: x: %2, y: %3").arg(ID).arg(scenePos.x()).arg(convertPos.y));
	}
	else {
		ui->statusBar->showMessage(QString("Entity ID: None"));
	}
}

void MainWindow::KeyUpdateSpritePosition(int ID, int key, float movePos)
{
	if (ID >= 0) {
		glm::vec3 updatedPos = p_spriteManager->UpdatePositionKeyPress(ID, key, movePos, customView, comboBox->currentIndex());
		entityManager->UpdateSpriteComponent(ID, updatedPos);

		// convert to 0,0 bottom left corner
		glm::vec2 convertPos = entityManager->ConvertCoords(updatedPos, ID);
		ui->statusBar->showMessage(QString("Entity ID: %1. Position: x: %2, y: %3").arg(ID).arg(updatedPos.x).arg(convertPos.y));
	}
}

void MainWindow::KeyRotateSprite(uint32_t ID, int key)
{
	if (ID >= 0) {
		float newRotation = p_spriteManager->RotateSprite(ID, key, comboBox->currentIndex());
		entityManager->UpdatePositionComponent(ID, newRotation);
		
		ui->statusBar->showMessage(QString("Entity ID: %1 Rotated by %2").arg(ID).arg(newRotation));
	}
}

void MainWindow::ShowEntityData(uint32_t ID)
{
	newEntityWin = new NewEntityWin(ID, customView, scene);
}

void MainWindow::PasteFromClipboard(int32_t id)
{
	SpriteManager::SpriteData sprite = entityManager->CreateEntityCopy(id);

	p_spriteManager->AddSpriteData(EntityManager::NEW_ENTITY, sprite, customView, scene);
	p_spriteManager->SwitchView(comboBox->currentIndex(), scene, customView);
}

void MainWindow::DeleteEntity(uint32_t ID)
{
	p_spriteManager->DeleteEntitySpriteData(ID);
	p_spriteManager->SwitchView(comboBox->currentIndex(), scene, customView);		// Actually, the view is not being switched, just refreshed to remove the deleted entity
	entityManager->DeleteEntityFromMap(ID);

	ui->statusBar->showMessage(QString("Entity ID: %1. Deleted Succesfully").arg(ID));
}

bool MainWindow::CheckForSave()
{
	const QMessageBox::StandardButton ret = QMessageBox::warning(this, "Application", "Save changes to entity data?",
		QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

	switch (ret) {
	case QMessageBox::Save:
		this->OnFileSave();
		break;
	case QMessageBox::Discard:
		break;
	case QMessageBox::Cancel:
		return false;
	}
	return true;
}

void MainWindow::SetIconStatus(bool status)
{
	m_saveAction->setEnabled(status);
	m_openFileAction->setEnabled(status);
	m_saveFileAction->setEnabled(status);
	m_copyAction->setEnabled(status);
	m_newEntAction->setEnabled(status);
	m_listEntAction->setEnabled(status);
}

