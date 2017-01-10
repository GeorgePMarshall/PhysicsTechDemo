#pragma once

#include <physx\PxPhysicsAPI.h>
#include <physx\PxScene.h>
#include <Camera.h>
#include "RagDoll.h"
#include "CharacterController.h"
#include "Fluid.h"


using namespace physx;

class PhysXScene
{
public:
	void ThreadedLoad();
	void Load();
	void Initialize();
	void Update(Camera& camera);
	void Draw();
	void Shutdown();


private:

	struct FilterGroup
	{
		enum Enum
		{
			player = (1 << 0),
			platform = (1 << 1),
			ground = (1 << 2)
		};
	};


	PxFoundation* physicsFoundation;
	PxPhysics* physics;
	PxScene* physicsScene;

	PxDefaultErrorCallback defaultErrorCallback;
	PxDefaultAllocator defaultAllocator;
	PxSimulationFilterShader defaultFilterShader;

	PxTransform* rootTransform;
	PxMaterial* physicsMaterial;
	PxMaterial* boxMaterial;
	PxCooking* physicsCooker;
	PxControllerManager* controllerManager;

	RagDoll ragDollCreator;
	CharacterController player;
	Fluid fluidEmmiter;

	float charge;


	void RenderGizmos(PxScene* physicsScene);

	void SetupPhysX();
	PxScene* CreateDefaultScene();
	void setupFiltering(PxRigidActor* actor, PxU32 filterGroup, PxU32 filterMask);
	void setShapeAsTrigger(PxRigidActor* actor);


	bool liquidEnabled;

public:
	void enableLiquid() { liquidEnabled = true; };
	
};
