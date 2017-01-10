#pragma once
#include <physx\PxPhysicsAPI.h>
#include "PhysXScene.h"

using namespace physx;

class CollisionCallback : public PxSimulationEventCallback
{
	PhysXScene* parentScene;


	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 numPairs);
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 numPairs);
	virtual void onConstraintBreak(PxConstraintInfo* constraintInfo, PxU32) {};
	virtual void onWake(PxActor**, PxU32) {};
	virtual void onSleep(PxActor**, PxU32) {};


public:

	CollisionCallback(PhysXScene* sceneWithLiquid) { parentScene = sceneWithLiquid; };

};