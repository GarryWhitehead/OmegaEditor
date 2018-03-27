#include "NewLevelWindow.h"
#include "SpriteManager.h"
#include "level_manager.h"
#include <qfile.h>
#include <qfiledialog.h>
#include <qmessagebox.h>

NewLevelWindow::NewLevelWindow()
{
	newLevelUi = new Ui::NewLevelForm();

	newLevelUi->setupUi(this);
	this->show();

	// cancel
	connect(newLevelUi->pushButton, &QPushButton::clicked, this, &NewLevelWindow::CloseNewWindow);

	// Ok - load texture, atlas and generate appropiate data
	connect(newLevelUi->pushButton_2, &QPushButton::clicked, this, &NewLevelWindow::OnPrepareLevel);

	// Main spritesheet 
	//toolbutton - load texture
	connect(newLevelUi->toolButton, &QToolButton::clicked, this, &NewLevelWindow::OnLoadMainTexture);

	//toolbutton - load texture atlas (.json)
	connect(newLevelUi->toolButton_2, &QToolButton::clicked, this, &NewLevelWindow::OnLoadMainAtlas);

	//Background spitesheer
	//toolbutton - load texture
	connect(newLevelUi->toolButton_3, &QToolButton::clicked, this, &NewLevelWindow::OnLoadBackgroundTexture);

	//toolbutton - load texture atlas (.json)
	connect(newLevelUi->toolButton_4, &QToolButton::clicked, this, &NewLevelWindow::OnLoadBackgroundAtlas);
}


NewLevelWindow::~NewLevelWindow()
{
	delete newLevelUi;
}

void NewLevelWindow::CloseNewWindow()
{
	this->close();
}

void NewLevelWindow::OnLoadMainTexture()
{
	mainTexFileName = QFileDialog::getOpenFileName(this, tr("Load Texture File"), "", tr("PNG (*.png);;All Files (*)"));
	if (mainTexFileName.isEmpty()) {
		QMessageBox msgBox;
		msgBox.setText("Error opening main sprite image file.");
		msgBox.exec();
	}
	else {
		newLevelUi->lineEdit->setText(mainTexFileName);
	}
}

void NewLevelWindow::OnLoadMainAtlas()
{
	mainAtlasFileName = QFileDialog::getOpenFileName(this, tr("Load Texture Atlas File"), "", tr("JSON (*.json);;All Files (*)"));
	if (mainAtlasFileName.isEmpty()) {
		QMessageBox msgBox;
		msgBox.setText("Error opening main texture atlas file.");
		msgBox.exec();
	}
	else {
		newLevelUi->lineEdit_2->setText(mainAtlasFileName);
	}
}

void NewLevelWindow::OnLoadBackgroundTexture()
{
	bgTexFileName = QFileDialog::getOpenFileName(this, tr("Load Texture File"), "", tr("PNG (*.png);;All Files (*)"));
	if (bgTexFileName.isEmpty()) {
		QMessageBox msgBox;
		msgBox.setText("Error opening background sprite image file.");
		msgBox.exec();
	}
	else {
		newLevelUi->lineEdit_4->setText(bgTexFileName);
	}
}

void NewLevelWindow::OnLoadBackgroundAtlas()
{
	bgAtlasFileName = QFileDialog::getOpenFileName(this, tr("Load Texture Atlas File"), "", tr("JSON (*.json);;All Files (*)"));
	if (bgAtlasFileName.isEmpty()) {
		QMessageBox msgBox;
		msgBox.setText("Error opening background texture atlas file.");
		msgBox.exec();
	}
	else {
		newLevelUi->lineEdit_3->setText(bgAtlasFileName);
	}
}

void NewLevelWindow::OnPrepareLevel()
{
	if (mainAtlasFileName.isEmpty() || mainTexFileName.isEmpty()) {
		QMessageBox msgBox;
		msgBox.setText("Data files not specified. Unable to create level.");
		msgBox.exec();
	}
	else {

		long levelTime = newLevelUi->spinBox->value();
		int worldWidth = newLevelUi->spinBox_2->value();
		int worldHeight = newLevelUi->spinBox_3->value();

		if (p_levelManager != nullptr) {
			delete p_levelManager;
		}

		p_levelManager = new LevelManager(mainAtlasFileName, mainTexFileName, bgAtlasFileName, bgTexFileName, levelTime, worldWidth, worldHeight);
		p_spriteManager = new SpriteManager(mainAtlasFileName, mainTexFileName, bgAtlasFileName, bgTexFileName, this);
		this->CloseNewWindow();
	}
}