#include "RagDoll.h"


PxArticulation* RagDoll::CreateRagDoll(PxPhysics* physics, PxTransform worldPos, float scale, PxMaterial* physicMaterial)
{
	RagdollNode* ragdollData[] =
	{
		new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), NO_PARENT,1,3,1,1,"lower spine"),
		new RagdollNode(PxQuat(PxPi, Z_AXIS), LOWER_SPINE, 1,1,-1,1,"left pelvis"),
		new RagdollNode(PxQuat(0, Z_AXIS), LOWER_SPINE, 1,1,-1, 1,"right pelvis"),
		new RagdollNode(PxQuat(PxPi / 2.0f + 0.2f, Z_AXIS),LEFT_PELVIS,5,2,-1,1,"L upper leg"),
		new RagdollNode(PxQuat(PxPi / 2.0f - 0.2f, Z_AXIS),RIGHT_PELVIS,5,2,-1,1,"R upper leg"),
		new RagdollNode(PxQuat(PxPi / 2.0f + 0.2f, Z_AXIS),LEFT_UPPER_LEG,5,1.75,-1,1,"L lower leg"),
		new RagdollNode(PxQuat(PxPi / 2.0f - 0.2f, Z_AXIS),RIGHT_UPPER_LEG,5,1.75,-1,1,"R lowerleg"),
		new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), LOWER_SPINE, 1, 3, 1, -1, "upper spine"),
		new RagdollNode(PxQuat(PxPi, Z_AXIS), UPPER_SPINE, 1, 1.5, 1, 1, "left clavicle"),
		new RagdollNode(PxQuat(0, Z_AXIS), UPPER_SPINE, 1, 1.5, 1, 1, "right clavicle"),
		new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), UPPER_SPINE, 1, 1, 1, -1, "neck"),
		new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), NECK, 1, 3, 1, -1, "HEAD"),
		new RagdollNode(PxQuat(PxPi - .3, Z_AXIS), LEFT_CLAVICLE, 3, 1.5, -1, 1, "left upper arm"),
		new RagdollNode(PxQuat(0.3, Z_AXIS), RIGHT_CLAVICLE, 3, 1.5, -1, 1, "right upper arm"),
		new RagdollNode(PxQuat(PxPi - .3, Z_AXIS), LEFT_UPPER_ARM, 3, 1, -1, 1, "left lower arm"),
		new RagdollNode(PxQuat(0.3, Z_AXIS), RIGHT_UPPER_ARM, 3, 1, -1, 1, "right lower arm"),
		NULL
	};

	PxArticulation* articulation = physics->createArticulation();
	RagdollNode** curNode = ragdollData;

	while(*curNode != NULL)
	{
		RagdollNode* curNodePtr = *curNode;
		RagdollNode* parentNode = nullptr;

		float radius = curNodePtr->radius * scale;
		float halfLength = curNodePtr->halfLength * scale;
		float childhalfLength = radius + halfLength;
		float parentHalfLength = 0;

		PxArticulationLink* parentLinkPtr = nullptr;
		curNodePtr->scaledGlobalPos = worldPos.p;

		if (curNodePtr->parentNodeIndex != NO_PARENT)
		{
			parentNode = *(ragdollData + curNodePtr->parentNodeIndex);
			parentLinkPtr = parentNode->linkPtr;
			parentHalfLength = (parentNode->radius + parentNode->halfLength) * scale;

			PxVec3 currentRelative = curNodePtr->childLinkPos * curNodePtr->globalRotaion.rotate(PxVec3(childhalfLength, 0, 0));
			PxVec3 parentRelative = -curNodePtr->parentLinkPos * parentNode->globalRotaion.rotate(PxVec3(parentHalfLength, 0, 0));
			curNodePtr->scaledGlobalPos = parentNode->scaledGlobalPos - (parentRelative + currentRelative);
		}

		PxTransform linkTransform = PxTransform(curNodePtr->scaledGlobalPos, curNodePtr->globalRotaion);
		PxArticulationLink* link = articulation->createLink(parentLinkPtr, linkTransform);
		link->setName("RagDoll");
		curNodePtr->linkPtr = link;

		float jointSpacing = .01f;
		float capsuleHalfLength = (halfLength > jointSpacing ? halfLength - jointSpacing : 0) + .01f;

		PxCapsuleGeometry capsule(radius, capsuleHalfLength);
		link->createShape(capsule, *physicMaterial);
		PxRigidBodyExt::updateMassAndInertia(*link, 50.0f);

		if (curNodePtr->parentNodeIndex != NO_PARENT)
		{
			PxArticulationJoint* joint = link->getInboundJoint();

			PxQuat frameRotation = parentNode->globalRotaion.getConjugate() * curNodePtr->globalRotaion;

			PxTransform parentConstraintFrame = PxTransform(PxVec3(curNodePtr->parentLinkPos * parentHalfLength, 0, 0), frameRotation);
			PxTransform curConstrainFrame = PxTransform(PxVec3(curNodePtr->childLinkPos * childhalfLength, 0, 0));

			joint->setParentPose(parentConstraintFrame);
			joint->setChildPose(curConstrainFrame);
			joint->setStiffness(20);
			joint->setSwingLimit(0.4f, 0.4f);
			joint->setSwingLimitEnabled(true);
			joint->setTwistLimit(-0.1f, 0.1f);
			joint->setTwistLimitEnabled(true);
		}


		curNode++;
	}

	return articulation;
}


