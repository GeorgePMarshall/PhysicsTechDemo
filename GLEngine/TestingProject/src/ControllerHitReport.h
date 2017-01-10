#pragma once
#include <physx\PxPhysicsAPI.h>
using namespace physx;

class ControllerHitReport : public PxUserControllerHitReport
{
	PxVec3 playerContactNormal;


public:
	virtual void onShapeHit(const PxControllerShapeHit &hit);
	virtual void onControllerHit(const PxControllersHit &hit);
	virtual void onObstacleHit(const PxControllerObstacleHit & hit);

	ControllerHitReport() : PxUserControllerHitReport() {};


	PxVec3 GetPlayerContactNormal() { return  playerContactNormal; }
	void clearPlayerContactNormal() { playerContactNormal = PxVec3(0); }


};