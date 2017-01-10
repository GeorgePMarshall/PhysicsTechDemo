#include "Fluid.h"
#include <gTime.h>

void Fluid::SetupParticleFluid(PxPhysics* physics, PxScene* physicsScene)
{
	particleFluid = physics->createParticleFluid(4000, false);
	
	particleFluid->setName("Liquid");
	particleFluid->setRestParticleDistance(.3f);
	particleFluid->setDynamicFriction(.1f);
	particleFluid->setStaticFriction(.1f);
	particleFluid->setDamping(.1f);
	particleFluid->setParticleMass(.1f);
	particleFluid->setRestitution(0);
	particleFluid->setParticleBaseFlag(PxParticleBaseFlag::eCOLLISION_TWOWAY, true);
	particleFluid->setStiffness(100);

	if (particleFluid)
	{
		physicsScene->addActor(*particleFluid);

		particleEmmiter = new ParticleFluidEmitter(4000, PxVec3(50, 10, 0), particleFluid, .1f);
		particleEmmiter->setStartVelocityRange(-.001f, -250.f, -.001f, .001f, -250.f, .001f);

	}
}

void Fluid::Update()
{
	if (particleEmmiter)
	{
		particleEmmiter->update(gTime::deltaTime());
		particleEmmiter->renderParticles();
	}
}

void Fluid::Draw()
{
	if (particleEmmiter)
	{
		particleEmmiter->renderParticles();
	}
}