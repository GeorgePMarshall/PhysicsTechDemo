#pragma once
#include <physx\PxPhysicsAPI.h>
#include "ParticleFluidEmitter.h"
using namespace physx;

class Fluid
{
	PxParticleFluid* particleFluid;
	ParticleFluidEmitter* particleEmmiter;

public:
	void SetupParticleFluid(PxPhysics* physics, PxScene* physicsScene);
	void Update();
	void Draw();
};