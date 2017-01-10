#include "CharacterController.h"
#include <Input.h>
#include <gTime.h>

void CharacterController::SetupCharacterController(PxScene* physicsScene, PxControllerManager* controllerManager, PxMaterial* physicsMaterial)
{
	hitReport = new ControllerHitReport();
	controllerManager = PxCreateControllerManager(*physicsScene);

	PxCapsuleControllerDesc desc;
	desc.height = 3.0f;
	desc.radius = 0.5f;
	desc.position.set(0, 0, 0);
	desc.material = physicsMaterial;
	desc.reportCallback = hitReport;
	desc.density = 10;

	characterController = controllerManager->createController(desc);
	characterController->setPosition(PxExtendedVec3(30, 0, 0));
	hitReport->clearPlayerContactNormal();

	characterController->getActor()->setName("Player");
	physicsScene->addActor(*characterController->getActor());

	gravity = -9.8f;
	movementSpeed = 5;
	rotationSpeed = 5;
}

void CharacterController::Update()
{
	PxVec3 velocity(0, 0, 0);


	if (hitReport->GetPlayerContactNormal().y > 0.3f)
	{
		onGround = true;
	}
	else
	{
		onGround = false;
		velocity.y += gravity * gTime::deltaTime();
	}




	if (Input::getInput()->getKey(GLFW_KEY_UP))
	{
		velocity.z -= movementSpeed * gTime::deltaTime();
	}

	if (Input::getInput()->getKey(GLFW_KEY_DOWN))
	{
		velocity.z += movementSpeed * gTime::deltaTime();
	}

	if (Input::getInput()->getKey(GLFW_KEY_LEFT))
	{
		velocity.x -= movementSpeed * gTime::deltaTime();
	}

	if (Input::getInput()->getKey(GLFW_KEY_RIGHT))
	{
		velocity.x += movementSpeed * gTime::deltaTime();
	}

	characterController->move(velocity, 0.001f, gTime::deltaTime(), filter);
}