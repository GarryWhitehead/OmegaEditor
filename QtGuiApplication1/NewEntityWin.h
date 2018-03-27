#pragma once

#include "ui_NewEntity.h"
#include "CustomGraphicsView.h"
#include "glm.hpp"

class NewEntityWin : public QWidget
{
	Q_OBJECT

public:

	NewEntityWin(int32_t entID, CustomGraphicsView *mainView, QGraphicsScene *mainScene);
	~NewEntityWin();

	void UpdateSpriteComponent(int index);

	// widget tab setter functions
	void SetPositionTabData(glm::vec3 pos, float rot);
	void SetVelocityTabData(glm::vec3 vel, glm::vec3 buoy);
	void SetAccelerationTabData(glm::vec3 accel, float mass);
	void SetAnimationTabData(uint32_t frameCount, uint32_t startIndex, uint32_t frameIndex, uint32_t animType);
	void SetHealthTabData(uint32_t maxHealth, uint32_t lives);
	void SetTimedTabData(bool isConstantLoop, long timeOn, long timeOff, uint32_t onType, uint32_t constantType, uint32_t loopedType);
	void SetCollisionTabData(uint32_t collisionType, float elasticity);
	void SetSpriteTabData(bool isActive, glm::vec2 size, glm::vec2 uv, uint32_t spriteIndex, uint32_t repeatCount, uint32_t spriteType);
	void SetPlayerTabData(glm::vec3 moveAccel, glm::vec3 moveVel, glm::vec3 jumpAccel, glm::vec3 jumpVel);

	// widget tab getter functions
	Qt::CheckState GetPositionTabData(glm::vec3& pos, float& rot);
	Qt::CheckState GetVelocityTabData(glm::vec3& vel, glm::vec3& buoy);
	Qt::CheckState GetAccelerationTabData(glm::vec3& accel, float& mass);
	Qt::CheckState GetAnimationTabData(uint32_t& frameCount, uint32_t& startIndex, uint32_t& frameIndex, uint32_t& animType);
	Qt::CheckState GetHealthTabData(uint32_t& maxHealth, uint32_t& lives);
	Qt::CheckState GetTimedTabData(bool& isConstantLoop, long& timeOn, long& timeOff, uint32_t& onType, uint32_t& constantType, uint32_t& loopedType);
	Qt::CheckState GetCollisionTabData(uint32_t& collisionType, float& elasticity);
	Qt::CheckState GetSpriteTabData(bool& isActive, glm::vec2& size, glm::vec2& uv, uint32_t& spriteIndex, uint32_t& repeatCount, uint32_t& spriteType);
	Qt::CheckState GetPlayerTabData(glm::vec3& moveAccel, glm::vec3& moveVel, glm::vec3& jumpAccel, glm::vec3& jumpVel);
	Qt::CheckState GetInputTabData();

private slots:
	
	void DeleteEntity();
	void CreateEntity(CustomGraphicsView *mainView, QGraphicsScene *mainScene);
	void EditEntity(uint32_t ID, CustomGraphicsView *mainView, QGraphicsScene *mainScene);
	void OnViewSpriteSheet();
	
public slots:
	void UpdateSpriteTab(int index);
	void UpdatePositionStatus(bool status);
	void UpdateAccelerationStatus(bool status);
	void UpdateVelocityStatus(bool status);
	void UpdatePlayerStatus(bool status);
	void UpdateHealthStatus(bool status);
	void UpdateTimedStatus(bool status);
	void UpdateCollisionStatus(bool status);
	void UpdateSpriteStatus(bool status);
	void UpdateAnimationStatus(bool status);
	void UpdateInputStatus(bool status);

signals:
	void SendSpriteData(int index);

private:
	Ui::NewEntForm *newEntUi;

	QWidget *viewSpriteWin;
};

