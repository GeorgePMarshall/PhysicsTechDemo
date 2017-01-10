#include "PhysicsManager.h"


void PhysicsManager::registerRigidbody(Rigidbody& rb)
{
	registeredRigidbodys.push_back(&rb);
}

void PhysicsManager::registerSpring(Spring& sp)
{
	registeredSprings.push_back(&sp);
}


void PhysicsManager::Update()
{
	for (Rigidbody* rb : registeredRigidbodys)
	{
		rb->Update();
	}

	for (Spring* sp : registeredSprings)
	{
		sp->Update();
	}


	for (int i = 0; i < registeredRigidbodys.size(); i++)
	{
		for (int j = i; j < registeredRigidbodys.size(); j++)
		{
			if (i == j)continue;
			registeredRigidbodys[i]->resolveColision(*registeredRigidbodys[j]);
		}
	}


}


