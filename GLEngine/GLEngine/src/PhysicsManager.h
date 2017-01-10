#pragma once
#include "Rigidbody.h"
#include "Colliders.h"
#include "GameObject.h"
#include "Spring.h"
#include <vector>

class PhysicsManager
{
	std::vector<Rigidbody*> registeredRigidbodys;
	std::vector<Spring*> registeredSprings;


public:

	void registerRigidbody(Rigidbody& rb);
	void registerSpring(Spring& sp);
	void Update();

	



};