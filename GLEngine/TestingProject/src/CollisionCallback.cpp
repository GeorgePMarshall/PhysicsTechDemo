#include "CollisionCallback.h"
#include <iostream>
#include <glm\glm.hpp>
#include <Gizmos.h>

void CollisionCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 numPairs)
{
	for (PxU32 i = 0; i < numPairs; i++)
	{
		const PxContactPair& cp = pairs[i];
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			std::cout << "collision between: ";
			std::cout << pairHeader.actors[0]->getName() << " and ";
			std::cout << pairHeader.actors[1]->getName() << std::endl;
		}
	}
}

void CollisionCallback::onTrigger(PxTriggerPair* pairs, PxU32 numPairs)
{
	for (PxU32 i = 0; i < numPairs; i++)
	{
		PxTriggerPair* pair = pairs + i;
		PxActor* triggerActor = pair->triggerActor;
		PxActor* otherActor = pair->otherActor;

		std::cout << otherActor->getName();
		std::cout << "Entered Trigger: ";
		std::cout << triggerActor->getName() << std::endl;

		if (triggerActor->getName() == "PhysxTrigger" && otherActor->getName() == "bobby")
		{
			parentScene->enableLiquid();
		}

	}
}
