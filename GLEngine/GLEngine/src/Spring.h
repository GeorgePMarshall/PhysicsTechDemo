#pragma once


class Rigidbody;

class Spring
{
	Rigidbody* object1;
	Rigidbody* object2;

	float springConstant;
	float damping;
	float restLength;

public:

	Spring(Rigidbody* object1, Rigidbody* object2, float springConstant, float damping);


	void Update();
	void Draw();

};