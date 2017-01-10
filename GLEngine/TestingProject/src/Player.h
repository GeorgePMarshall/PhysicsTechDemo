#pragma once
#include <GameObject.h>
#include <Gizmos.h>
#include <Rigidbody.h>
#include <PhysicsManager.h>

class Player : public gameObject
{

public:
	Rigidbody rigidbody;
	void Initialize();
	void Update();
	void Draw();


};