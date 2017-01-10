#include "ControllerHitReport.h"

void ControllerHitReport::onShapeHit(const PxControllerShapeHit &hit)
{
	PxRigidActor* actor = hit.shape->getActor();
	playerContactNormal = hit.worldNormal;
	PxRigidDynamic* myActor = actor->is<PxRigidDynamic>();
	if (myActor)
	{

	}
}
void ControllerHitReport::onControllerHit(const PxControllersHit &hit)
{

}
void ControllerHitReport::onObstacleHit(const PxControllerObstacleHit & hit)
{

}