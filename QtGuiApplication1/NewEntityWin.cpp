#include "NewEntityWin.h"
#include "EntityManager.h"
#include "SpriteManager.h"
#include "SpriteViewWin.h"
#include <qmessagebox.h>
#include <qgraphicsview.h>
#include "components/sprite.h"
#include "components/collision.h"
#include "utility/file_log.h"
#include <qevent.h>

// sending -1 as a ID parameter indicates to the function to create a new entity instance. A non-negative number will
// display the component settings of the particular entity and will erase the old entity and add the edited version to the map

NewEntityWin::NewEntityWin(int32_t entID, CustomGraphicsView *mainView, QGraphicsScene *mainScene)
{
	newEntUi = new Ui::NewEntForm();

	newEntUi->setupUi(this);
	this->show();

	// update tabs to reflect selections
	this->UpdateSpriteTab(0);

	// window signal/slot connections =============================================================================================================================================================================================
	// image index sprite tab
	connect(newEntUi->spinBox_8, SIGNAL(valueChanged(int)), this, SLOT(UpdateSpriteTab(int)));
	connect(newEntUi->comboBox_6, SIGNAL(activated(int)), this, SLOT(UpdateSpriteTab(int)));

	// Enable/disable components
	connect(newEntUi->checkBox, SIGNAL(clicked(bool)), this, SLOT(UpdatePositionStatus(bool)));
	connect(newEntUi->checkBox_3, SIGNAL(clicked(bool)), this, SLOT(UpdateVelocityStatus(bool)));
	connect(newEntUi->checkBox_4, SIGNAL(clicked(bool)), this, SLOT(UpdateAccelerationStatus(bool)));
	connect(newEntUi->checkBox_5, SIGNAL(clicked(bool)), this, SLOT(UpdateAnimationStatus(bool)));
	connect(newEntUi->checkBox_6, SIGNAL(clicked(bool)), this, SLOT(UpdateHealthStatus(bool)));
	connect(newEntUi->checkBox_7, SIGNAL(clicked(bool)), this, SLOT(UpdateTimedStatus(bool)));
	connect(newEntUi->checkBox_8, SIGNAL(clicked(bool)), this, SLOT(UpdateCollisionStatus(bool)));
	connect(newEntUi->checkBox_9, SIGNAL(clicked(bool)), this, SLOT(UpdateSpriteStatus(bool)));
	connect(newEntUi->checkBox_11, SIGNAL(clicked(bool)), this, SLOT(UpdatePlayerStatus(bool)));

	// cancel pushbutton
	connect(newEntUi->pushButton, &QPushButton::clicked, this, &NewEntityWin::close);

	// sprite sheet view
	connect(newEntUi->pushButton_4, &QPushButton::clicked, this, &NewEntityWin::OnViewSpriteSheet);

	if (entID >= 0) {
		entityManager->AddEntityDataToWindow(entID, this);
		this->setWindowTitle(QString("Edit - Entity ID: %1").arg(entID));
		// Ok clicked - edit entity 
		connect(newEntUi->pushButton_2, &QPushButton::clicked, this, [=]() { EditEntity(entID, mainView, mainScene); });
	}
	else {
		// Ok clicked - create entity, components and add to map
		this->setWindowTitle(QString("New Entity"));
		connect(newEntUi->pushButton_2, &QPushButton::clicked, this, [=]() { CreateEntity(mainView, mainScene); });
	}
}

NewEntityWin::~NewEntityWin()
{
	delete newEntUi;
}

void NewEntityWin::DeleteEntity()
{

}

void NewEntityWin::UpdateSpriteTab(int index)
{
	int imageIndex = newEntUi->spinBox_8->value();
	int spriteType = newEntUi->comboBox_6->currentIndex();
	SpriteManager::AtlasData atlasData;
	int max = 0;

	if (spriteType == MAIN_SPRITE) {
		max = p_spriteManager->GetMainAtlasDataSize() - 1;
		if (imageIndex > max) {
			newEntUi->spinBox_8->setValue(max);
			imageIndex = max;
		}
		atlasData = p_spriteManager->GetMainAtlasData(imageIndex);
		
	}
	else if (spriteType == BACKGROUND_SPRITE) {
		max = p_spriteManager->GetBackgroundAtlasDataSize() - 1; 
		if (imageIndex > max) {
			newEntUi->spinBox_8->setValue(max);
			imageIndex = max;
		}
		atlasData = p_spriteManager->GetBackgroundAtlasData(imageIndex);
		
	}
	newEntUi->doubleSpinBox_41->setValue(atlasData.size.x);			// width and height of sprite
	newEntUi->doubleSpinBox_40->setValue(atlasData.size.y);
	newEntUi->doubleSpinBox_43->setValue(atlasData.uv.x);			// x and y tex coords
	newEntUi->doubleSpinBox_42->setValue(atlasData.uv.y);
	this->update();
}

void NewEntityWin::CreateEntity(CustomGraphicsView *mainView, QGraphicsScene *mainScene)
{
	if (entityManager->AddEntity(EntityManager::NEW_ENTITY, 0, this, mainView, mainScene)) {
		QMessageBox msgBox;
		msgBox.setText("Entity succesfully created.");
		msgBox.exec();

		this->close();
	}
}

void NewEntityWin::EditEntity(uint32_t ID, CustomGraphicsView *mainView, QGraphicsScene *mainScene)
{
	if (entityManager->AddEntity(EntityManager::EDIT_ENTITY, ID, this, mainView, mainScene)) {
		QMessageBox msgBox;
		msgBox.setText("Entity succesfully edited.");
		msgBox.exec();

		this->close();
	}
}

void NewEntityWin::OnViewSpriteSheet()
{
	viewSpriteWin = new SpriteViewWin(this, newEntUi->comboBox_6->currentIndex());
}

void NewEntityWin::UpdateSpriteComponent(int index)
{
	if (index < 0) {
		return;
	}
	newEntUi->spinBox_8->setValue(index);
	this->UpdateSpriteTab(index);
	this->update();
}

// Enabled/Disaled component tab functions ==================================================================================================================================================================================
void NewEntityWin::UpdatePositionStatus(bool status)
{
	newEntUi->checkBox->setChecked(status);
	newEntUi->doubleSpinBox_22->setEnabled(status);
	newEntUi->doubleSpinBox_23->setEnabled(status);
	newEntUi->doubleSpinBox_4->setEnabled(status);
	newEntUi->radioButton->setEnabled(status);
}

void NewEntityWin::UpdateVelocityStatus(bool status)
{
	newEntUi->checkBox_3->setChecked(status);
	newEntUi->doubleSpinBox_25->setEnabled(status);
	newEntUi->doubleSpinBox_26->setEnabled(status);
	newEntUi->doubleSpinBox_28->setEnabled(status);
	newEntUi->doubleSpinBox_29->setEnabled(status);
}

void NewEntityWin::UpdateAccelerationStatus(bool status)
{
	newEntUi->checkBox_4->setChecked(status);
	newEntUi->doubleSpinBox_31->setEnabled(status);
	newEntUi->doubleSpinBox_32->setEnabled(status);
	newEntUi->doubleSpinBox_33->setEnabled(status);
}

void NewEntityWin::UpdateAnimationStatus(bool status)
{
	newEntUi->checkBox_5->setChecked(status);
	newEntUi->spinBox->setEnabled(status);
	newEntUi->spinBox_2->setEnabled(status);
	newEntUi->spinBox_3->setEnabled(status);
	newEntUi->comboBox->setEnabled(status);
}

void NewEntityWin::UpdateHealthStatus(bool status)
{
	newEntUi->checkBox_6->setChecked(status);
	newEntUi->spinBox_4->setEnabled(status);
	newEntUi->spinBox_5->setEnabled(status);
}

void NewEntityWin::UpdateTimedStatus(bool status)
{
	newEntUi->checkBox_7->setChecked(status);
	newEntUi->spinBox_6->setEnabled(status);
	newEntUi->spinBox_7->setEnabled(status);
	newEntUi->spinBox_3->setEnabled(status);
	newEntUi->comboBox->setEnabled(status);
	newEntUi->radioButton_2->setEnabled(status);
	newEntUi->comboBox_2->setEnabled(status);
	newEntUi->comboBox_3->setEnabled(status);
	newEntUi->comboBox_4->setEnabled(status);
}

void NewEntityWin::UpdateCollisionStatus(bool status)
{
	if (status) {
		this->UpdateAccelerationStatus(true);
		this->UpdateVelocityStatus(true);
	}

	newEntUi->checkBox_8->setChecked(status);
	newEntUi->doubleSpinBox_37->setEnabled(status);
	newEntUi->checkBox_2->setEnabled(status);
	newEntUi->checkBox_13->setEnabled(status);
	newEntUi->checkBox_14->setEnabled(status);
	newEntUi->checkBox_15->setEnabled(status);
	newEntUi->checkBox_16->setEnabled(status);
	newEntUi->checkBox_17->setEnabled(status);
}

void NewEntityWin::UpdateSpriteStatus(bool status)
{
	newEntUi->checkBox_9->setChecked(status);
	newEntUi->doubleSpinBox_40->setEnabled(status);
	newEntUi->doubleSpinBox_41->setEnabled(status);
	newEntUi->doubleSpinBox_42->setEnabled(status);
	newEntUi->doubleSpinBox_43->setEnabled(status);
	newEntUi->spinBox_8->setEnabled(status);
	newEntUi->spinBox_9->setEnabled(status);
	newEntUi->comboBox_6->setEnabled(status);
	newEntUi->pushButton_4->setEnabled(status);
	newEntUi->radioButton_3->setEnabled(status);
}

void NewEntityWin::UpdatePlayerStatus(bool status)
{
	if (status) {
		this->UpdateHealthStatus(true);
		this->UpdateCollisionStatus(true);
		this->UpdateInputStatus(true);
	}

	newEntUi->checkBox_11->setChecked(status);
	newEntUi->doubleSpinBox_64->setEnabled(status);
	newEntUi->doubleSpinBox_65->setEnabled(status);
	newEntUi->doubleSpinBox_66->setEnabled(status);
	newEntUi->doubleSpinBox_67->setEnabled(status);
}

void NewEntityWin::UpdateInputStatus(bool status)
{
	newEntUi->checkBox_12->setChecked(status);
}

// ========================================================================================================================
// Functions for retrieving data from component tabs

Qt::CheckState NewEntityWin::GetPositionTabData(glm::vec3& pos, float& rot)
{
	Qt::CheckState state = newEntUi->checkBox->checkState();
	if (state == Qt::Checked) {
		pos.x = float(newEntUi->doubleSpinBox_23->value());			// x pos
		pos.y = float(newEntUi->doubleSpinBox_22->value());			// y pos
		rot = float(newEntUi->doubleSpinBox_4->value());		// rotation
	}
	return state;
}

Qt::CheckState NewEntityWin::GetVelocityTabData(glm::vec3& vel, glm::vec3& buoy)
{
	Qt::CheckState state = newEntUi->checkBox_3->checkState();
	if (state == Qt::Checked) {
		vel.x = float(newEntUi->doubleSpinBox_26->value());			// x vel
		vel.y = float(newEntUi->doubleSpinBox_25->value());			// y vel
		buoy.x = float(newEntUi->doubleSpinBox_29->value());			// bouyancy.x
		buoy.y = float(newEntUi->doubleSpinBox_28->value());			// bouyancy.y
	}
	return state;
}

Qt::CheckState NewEntityWin::GetAccelerationTabData(glm::vec3& accel, float& mass)
{
	Qt::CheckState state = newEntUi->checkBox_4->checkState();
	if (state) {
		accel.x = float(newEntUi->doubleSpinBox_32->value());			// x accel
		accel.y = float(newEntUi->doubleSpinBox_31->value());			// y accel
		mass = float(newEntUi->doubleSpinBox_33->value());		// mass
	}
	return state;
}

Qt::CheckState NewEntityWin::GetAnimationTabData(uint32_t& frameCount, uint32_t& startIndex, uint32_t& frameIndex, uint32_t& animType)
{
	Qt::CheckState state = newEntUi->checkBox_5->checkState();
	if (state) {
		frameCount = uint32_t(newEntUi->spinBox->value());			// number of frames
		startIndex = uint32_t(newEntUi->spinBox_2->value());			// start frame in atlas
		frameIndex = uint32_t(newEntUi->spinBox_3->value());			// 
		animType = newEntUi->comboBox->currentIndex();
	}
	return state;
}

Qt::CheckState NewEntityWin::GetHealthTabData(uint32_t& maxHealth, uint32_t& lives)
{
	Qt::CheckState state = newEntUi->checkBox_6->checkState();
	if (state) {
		maxHealth = newEntUi->spinBox_5->value();						// starting health
		lives = newEntUi->spinBox_4->value();							// number of lives
	}
	return state;
}

Qt::CheckState NewEntityWin::GetTimedTabData(bool& isConstantLoop, long& timeOn, long& timeOff, uint32_t& onType, uint32_t& constantType, uint32_t& loopedType)
{
	Qt::CheckState state = newEntUi->checkBox_7->checkState();
	if (state == Qt::Checked) {

		timeOn = long(newEntUi->spinBox_6->value());						// time on 
		onType = newEntUi->comboBox_4->currentIndex();
		isConstantLoop = newEntUi->radioButton_2->isChecked();
		if (isConstantLoop) {												// constantly loop on/off
			timeOff = long(newEntUi->spinBox_6->value());
			loopedType = newEntUi->comboBox_2->currentIndex();
		}
		else {															// constant off logic
			constantType = newEntUi->comboBox_3->currentIndex();
		}
	}
	return state;
}

Qt::CheckState NewEntityWin::GetCollisionTabData(uint32_t&collBits, float& elasticity)
{
	collBits = 0;
	Qt::CheckState state = newEntUi->checkBox_8->checkState();
	if (state == Qt::Checked) {
		if (newEntUi->checkBox_2->isChecked()) {
			collBits |= COLL_LETHAL_BIT;
		}
		if (newEntUi->checkBox_13->isChecked()) {
			collBits |= COLL_FLOOR_BIT;
		}
		if (newEntUi->checkBox_14->isChecked()) {
			collBits |= COLL_IMPULSE_BIT;
		}
		if (newEntUi->checkBox_15->isChecked()) {
			collBits |= COLL_BREAKABLE_BIT;
		}
		if (newEntUi->checkBox_16->isChecked()) {
			collBits |= COLL_HIDING_BIT;
		}
		if (newEntUi->checkBox_17->isChecked()) {
			collBits |= COLL_MOVING_BIT;
		}
		elasticity = float(newEntUi->doubleSpinBox_37->value());			// elasticity of collision (if selected in type)
	}
	return state;
}

Qt::CheckState NewEntityWin::GetSpriteTabData(bool& isActive, glm::vec2& size, glm::vec2& uv, uint32_t& spriteIndex, uint32_t& repeatCount, uint32_t& spriteType)
{
	Qt::CheckState state = newEntUi->checkBox_9->checkState();
	if (state == Qt::Checked) {
		size.x = float(newEntUi->doubleSpinBox_41->value());			// width and height of sprite
		size.y = float(newEntUi->doubleSpinBox_40->value());
		uv.x = float(newEntUi->doubleSpinBox_43->value());			// tex coords for spritesheet
		uv.y = float(newEntUi->doubleSpinBox_42->value());
		isActive = newEntUi->radioButton_3->isChecked();
		spriteIndex = newEntUi->spinBox_8->value();				// sprite index within atlas
		repeatCount = newEntUi->spinBox_9->value();					// number of times to repeat sprite
		spriteType = newEntUi->comboBox_6->currentIndex();
	}
	return state;
}

Qt::CheckState NewEntityWin::GetPlayerTabData(glm::vec3& moveAccel, glm::vec3& moveVel, glm::vec3& jumpAccel, glm::vec3& jumpVel)
{
	Qt::CheckState state = newEntUi->checkBox_11->checkState();
	if (state == Qt::Checked) {
		moveAccel.x = float(newEntUi->doubleSpinBox_67->value());			// movement acceleration and velocity
		moveVel.x = float(newEntUi->doubleSpinBox_66->value());
		jumpAccel.y = float(newEntUi->doubleSpinBox_65->value());			// jump acceleration and velocity
		jumpVel.y = float(newEntUi->doubleSpinBox_64->value());
	}
	return state;
}

Qt::CheckState NewEntityWin::GetInputTabData()
{
	return newEntUi->checkBox_12->checkState();
}

// ========================================================================================================================
// Functions for setting window tab widgtes

void NewEntityWin::SetPositionTabData(glm::vec3 pos, float rot)
{
	newEntUi->doubleSpinBox_23->setValue(pos.x);											// x pos
	newEntUi->doubleSpinBox_22->setValue(pos.y);											// y pos
	newEntUi->doubleSpinBox_4->setValue(rot);												// rotation
}

void NewEntityWin::SetVelocityTabData(glm::vec3 vel, glm::vec3 buoy)
{
	newEntUi->doubleSpinBox_26->setValue(vel.x);										// x vel
	newEntUi->doubleSpinBox_25->setValue(vel.y);										// y vel
	newEntUi->doubleSpinBox_29->setValue(buoy.x);										// bouyancy.x
	
	if (buoy.y == 0.0f) {
		buoy.y = 9.8f;
	}
	newEntUi->doubleSpinBox_28->setValue(buoy.y);										// bouyancy.y
}

void NewEntityWin::SetAccelerationTabData(glm::vec3 accel, float mass)
{
	newEntUi->doubleSpinBox_32->setValue(accel.x);										// x accel
	newEntUi->doubleSpinBox_31->setValue(accel.y);										// y accel
	newEntUi->doubleSpinBox_33->setValue(mass);
}

void NewEntityWin::SetAnimationTabData(uint32_t frameCount, uint32_t startIndex, uint32_t frameIndex, uint32_t animType)
{
	newEntUi->spinBox->setValue(frameCount);											// number of frames
	newEntUi->spinBox_2->setValue(startIndex);											// start frame in atlas
	newEntUi->spinBox_3->setValue(frameIndex);											// 
	newEntUi->comboBox->setCurrentIndex(animType);
}

void NewEntityWin::SetHealthTabData(uint32_t maxHealth, uint32_t lives)
{
	if (maxHealth == 0) {
		maxHealth = 30;
	}
	if (lives == 0) {
		lives = 3;
	}
	newEntUi->spinBox_5->setValue(maxHealth);											// starting health
	newEntUi->spinBox_4->setValue(lives);												// number of lives
}

void NewEntityWin::SetTimedTabData(bool isConstantLoop, long timeOn, long timeOff, uint32_t onType, uint32_t constantType, uint32_t loopedType)
{
	newEntUi->spinBox_6->setValue(timeOn);								// time on 
	newEntUi->comboBox_4->setCurrentIndex(onType);

	if (isConstantLoop) {												// constantly loop on/off
		newEntUi->radioButton_2->setChecked(true);
		newEntUi->spinBox_6->setValue(timeOff);
		newEntUi->comboBox_2->setCurrentIndex(loopedType);
	}
	else {	
		newEntUi->radioButton_2->setChecked(false);
		newEntUi->comboBox_3->setCurrentIndex(constantType);
	}
}

void NewEntityWin::SetCollisionTabData(uint32_t collBits, float elasticity)
{
	if (collBits & COLL_LETHAL_BIT) {
		newEntUi->checkBox_2->setChecked(true);
	}
	else {
		newEntUi->checkBox_2->setChecked(false);
	}
	if (collBits & COLL_FLOOR_BIT) {
		newEntUi->checkBox_13->setChecked(true);
	}
	else {
		newEntUi->checkBox_13->setChecked(false);
	}
	if (collBits & COLL_IMPULSE_BIT) {
		newEntUi->checkBox_14->setChecked(true);
	}
	else {
		newEntUi->checkBox_14->setChecked(false);
	}
	if (collBits & COLL_BREAKABLE_BIT) {
		newEntUi->checkBox_15->setChecked(true);
	}
	else {
		newEntUi->checkBox_15->setChecked(false);
	}
	if (collBits & COLL_HIDING_BIT) {
		newEntUi->checkBox_16->setChecked(true);
	}
	else {
		newEntUi->checkBox_16->setChecked(false);
	}
	if (collBits & COLL_MOVING_BIT) {
		newEntUi->checkBox_17->setChecked(true);
	}
	else {
		newEntUi->checkBox_17->setChecked(false);
	}
	newEntUi->doubleSpinBox_37->setValue(elasticity);
}

void NewEntityWin::SetSpriteTabData(bool isActive, glm::vec2 size, glm::vec2 uv, uint32_t spriteIndex, uint32_t repeatCount, uint32_t spriteType)
{
	newEntUi->doubleSpinBox_41->setValue(size.x);						// width and height of sprite
	newEntUi->doubleSpinBox_40->setValue(size.y);
	newEntUi->doubleSpinBox_43->setValue(uv.x);							// uv tex coords
	newEntUi->doubleSpinBox_42->setValue(uv.y);

	if (isActive) {
		newEntUi->radioButton_3->setChecked(true);
	}
	else {
		newEntUi->radioButton_3->setChecked(false);
	}

	newEntUi->spinBox_8->setValue(spriteIndex);							// sprite index within atlas
	newEntUi->spinBox_9->setValue(repeatCount);							// number of times to repeat sprite
	newEntUi->comboBox_6->setCurrentIndex(spriteType);
}

void NewEntityWin::SetPlayerTabData(glm::vec3 moveAccel, glm::vec3 moveVel, glm::vec3 jumpAccel, glm::vec3 jumpVel)
{
	newEntUi->doubleSpinBox_67->setValue(moveAccel.x);					// movement acceleration and velocity
	newEntUi->doubleSpinBox_66->setValue(moveVel.x);
	newEntUi->doubleSpinBox_65->setValue(jumpAccel.y);					// jump acceleration and velocity
	newEntUi->doubleSpinBox_64->setValue(jumpVel.y);
}