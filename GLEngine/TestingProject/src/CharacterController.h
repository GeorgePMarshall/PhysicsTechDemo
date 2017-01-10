#pragma once
#include <physx\PxPhysicsAPI.h>
#include "ControllerHitReport.h"

class CharacterController
{
	ControllerHitReport* hitReport;
	PxController* characterController;
	PxControllerFilters filter;

	float gravity;
	float movementSpeed;
	float rotationSpeed;
	bool onGround;

public:

	void SetupCharacterController(PxScene* physicsScene, PxControllerManager* controllerManager, PxMaterial* physicsMaterial);
	void Update();
};