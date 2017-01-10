#pragma once
#include <physx\PxPhysicsAPI.h>

using namespace physx;

class RagDoll
{
	enum RagDollParts
	{
		NO_PARENT = -1,
		LOWER_SPINE,
		LEFT_PELVIS,
		RIGHT_PELVIS,
		LEFT_UPPER_LEG,
		RIGHT_UPPER_LEG,
		LEFT_LOWER_LEG,
		RIGHT_LOWER_LEG,
		UPPER_SPINE,
		LEFT_CLAVICLE,
		RIGHT_CLAVICLE,
		NECK,
		HEAD,
		LEFT_UPPER_ARM,
		RIGHT_UPPER_ARM,
		LEFT_LOWER_ARM,
		RIGHT_LOWER_ARM,
	};

	const PxVec3 X_AXIS = PxVec3(1, 0, 0);
	const PxVec3 Y_AXIS = PxVec3(0, 1, 0);
	const PxVec3 Z_AXIS = PxVec3(0, 0, 1);

	struct RagdollNode
	{
		PxQuat globalRotaion;
		PxVec3 scaledGlobalPos;

		int parentNodeIndex;
		float halfLength;
		float radius;
		float parentLinkPos;
		float childLinkPos;
		char* name;
		PxArticulationLink* linkPtr;

		RagdollNode(PxQuat globalRotaion, int parentNodeIndex, float halfLength, float radius, float parentLinkPos, float childLinkPos, char* name)
		{
			this->globalRotaion	= globalRotaion;
			this->parentNodeIndex = parentNodeIndex;
			this->halfLength = halfLength;
			this->radius = radius;
			this->parentLinkPos = parentLinkPos;
			this->childLinkPos = childLinkPos;
			this->name = name;
		}
	};
public:
	PxArticulation* CreateRagDoll(PxPhysics* physics, PxTransform worldPos, float scale, PxMaterial* physicMaterial);

};