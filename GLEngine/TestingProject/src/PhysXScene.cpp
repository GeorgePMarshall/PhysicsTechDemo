#include "PhysXScene.h"
#include <gTime.h>
#include <Input.h>
#include "Gizmos.h"
#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>
#include "CollisionCallback.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

void PhysXScene::Initialize()
{
	SetupPhysX();
	physicsScene = CreateDefaultScene();

	rootTransform = new PxTransform(PxVec3(50, 0, 0));
	//physicsScene->shiftOrigin(PxVec3(10, 0, 0));

	PxSimulationEventCallback* collisionCallback = new CollisionCallback(this);
	physicsScene->setSimulationEventCallback(collisionCallback);

	PxTransform planeTransform = PxTransform(PxVec3(0, 0, 0), PxQuat(PxHalfPi * 1.0f, PxVec3(0.0f, 0.0f, 1.0f)));
	PxRigidStatic* plane = PxCreateStatic(*physics, *rootTransform * planeTransform, PxPlaneGeometry(), *physicsMaterial);
	plane->setName("Ground");
	setupFiltering(plane, FilterGroup::ground, FilterGroup::player);
	physicsScene->addActor(*plane);

	PxBoxGeometry boxGeom(2, 2, 2);
	PxTransform boxTransform(PxVec3(5, 5, 5));
	PxRigidDynamic* boxActor = PxCreateDynamic(*physics, *rootTransform * boxTransform, boxGeom, *physicsMaterial, 10.0f);
	boxActor->setName("Box");
	physicsScene->addActor(*boxActor);

	PxArticulation* ragDollArticulation;
	PxTransform ragDollTransform(PxVec3(0, 5, 0));
	ragDollArticulation = ragDollCreator.CreateRagDoll(physics, *rootTransform * ragDollTransform, .1f, physicsMaterial);
	ragDollArticulation->setName("Ragdoll");
	physicsScene->addArticulation(*ragDollArticulation);

	PxBoxGeometry triggerBoxGeom(2, 2, 2);
	PxTransform triggerTransform(PxVec3(3, 5, 10));
	PxRigidStatic* trigger = PxCreateStatic(*physics, *rootTransform * triggerTransform, triggerBoxGeom, *physicsMaterial);
	trigger->setName("PhysxTrigger");
	setShapeAsTrigger(trigger);
	physicsScene->addActor(*trigger);

	player.SetupCharacterController(physicsScene, controllerManager, physicsMaterial);

	fluidEmmiter.SetupParticleFluid(physics, physicsScene);
}

void PhysXScene::ThreadedLoad()
{

}

void PhysXScene::Load()
{

}

void PhysXScene::Update(Camera& camera)
{
	
	if (Input::getInput()->getKey(GLFW_KEY_R))
	{
		charge += 50 * gTime::deltaTime();
	}

	if (Input::getInput()->getKeyUp(GLFW_KEY_R))
	{
		PxSphereGeometry sphereGeom(1);
		PxTransform sphereTransform(PxVec3(camera.getPosition().x, camera.getPosition().y, camera.getPosition().z));
		PxRigidDynamic* sphereActor = PxCreateDynamic(*physics, sphereTransform, sphereGeom, *physicsMaterial, 10.0f);

		vec3 velocity(camera.getWorldTransform()[2].xyz * -1 * charge);
		sphereActor->setLinearVelocity(PxVec3(velocity.x, velocity.y, velocity.z), true);
		setupFiltering(sphereActor, FilterGroup::player, FilterGroup::ground);
		sphereActor->setName("bobby");
		physicsScene->addActor(*sphereActor);

		charge = 0.0f;
	}
	
	
	
	player.Update();

	if(liquidEnabled)
		fluidEmmiter.Update();
	
	if (gTime::deltaTime() > 0)
	{
		physicsScene->simulate(gTime::deltaTime() > 0.033f ? 0.033f : gTime::deltaTime());
		while (!physicsScene->fetchResults());
	}
}

void PhysXScene::Draw()
{
	RenderGizmos(physicsScene);
	fluidEmmiter.Draw();
}

void PhysXScene::Shutdown()
{
	delete rootTransform;

	physicsScene->release();
	physicsFoundation->release();
	physics->release();
}




//AIE's PhysX GizmosRender
void AddWidget(PxShape* shape, PxRigidActor* actor, vec4 geo_color)
{
	PxTransform full_transform = PxShapeExt::getGlobalPose(*shape, *actor);
	vec3 actor_position(full_transform.p.x, full_transform.p.y, full_transform.p.z);
	glm::quat actor_rotation(full_transform.q.w, full_transform.q.x, full_transform.q.y, full_transform.q.z);
	glm::mat4 rot(actor_rotation);

	mat4 rotate_matrix = glm::rotate(10.f, glm::vec3(7, 7, 7));

	PxGeometryType::Enum geo_type = shape->getGeometryType();

	switch (geo_type)
	{
	case (PxGeometryType::eBOX):
	{
		PxBoxGeometry geo;
		shape->getBoxGeometry(geo);
		vec3 extents(geo.halfExtents.x, geo.halfExtents.y, geo.halfExtents.z);
		Gizmos::addAABBFilled(actor_position, extents, geo_color, &rot);
	} break;
	case (PxGeometryType::eCAPSULE):
	{
		PxCapsuleGeometry geo;
		shape->getCapsuleGeometry(geo);
		Gizmos::addCapsule(actor_position, geo.halfHeight * 2, geo.radius, 16, 16, vec4(0.1f, 0.3f, 0.3f, 1.f), &rot);
	} break;
	case (PxGeometryType::eSPHERE):
	{
		PxSphereGeometry geo;
		shape->getSphereGeometry(geo);
		Gizmos::addSphereFilled(actor_position, geo.radius, 16, 16, geo_color, &rot);
	} break;
	case (PxGeometryType::ePLANE):
	{
		Gizmos::addAABBFilled(actor_position, vec3(20, 0, 20), vec4(0.5f, 0.9f, 0.5f, 1.f));
	} break;
	}
}
void PhysXScene::RenderGizmos(PxScene* physics_scene)
{
	PxActorTypeFlags desiredTypes = PxActorTypeFlag::eRIGID_STATIC | PxActorTypeFlag::eRIGID_DYNAMIC;
	PxU32 actor_count = physics_scene->getNbActors(desiredTypes);
	PxActor** actor_list = new PxActor*[actor_count];
	physics_scene->getActors(desiredTypes, actor_list, actor_count);

	vec4 geo_color(1, 0, 0, 1);
	for (int actor_index = 0;
		actor_index < (int)actor_count;
		++actor_index)
	{
		PxActor* curr_actor = actor_list[actor_index];
		if (curr_actor->isRigidActor())
		{
			PxRigidActor* rigid_actor = (PxRigidActor*)curr_actor;
			PxU32 shape_count = rigid_actor->getNbShapes();
			PxShape** shapes = new PxShape*[shape_count];
			rigid_actor->getShapes(shapes, shape_count);

			for (int shape_index = 0;
				shape_index < (int)shape_count;
				++shape_index)
			{
				PxShape* curr_shape = shapes[shape_index];
				AddWidget(curr_shape, rigid_actor, geo_color);
			}

			delete[]shapes;
		}
	}

	delete[] actor_list;

	int articulation_count = physics_scene->getNbArticulations();

	for (int a = 0; a < articulation_count; ++a)
	{
		PxArticulation* articulation;
		physics_scene->getArticulations(&articulation, 1, a);

		int link_count = articulation->getNbLinks();

		PxArticulationLink** links = new PxArticulationLink*[link_count];
		articulation->getLinks(links, link_count);

		for (int l = 0; l < link_count; ++l)
		{
			PxArticulationLink* link = links[l];
			int shape_count = link->getNbShapes();

			for (int s = 0; s < shape_count; ++s)
			{
				PxShape* shape;
				link->getShapes(&shape, 1, s);
				AddWidget(shape, link, geo_color);
			}
		}
		delete[] links;
	}
}

PxFilterFlags filterShader(PxFilterObjectAttributes atribute0, PxFilterData filterData0, PxFilterObjectAttributes atribute1, PxFilterData filterData1, PxPairFlags& pairFlags, const void* constantBlack, PxU32 constantBlockSize)
{
	if (PxFilterObjectIsTrigger(atribute0) || PxFilterObjectIsTrigger(atribute1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}

	pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
	{
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;
	}

	return PxFilterFlag::eDEFAULT;

}

void PhysXScene::SetupPhysX()
{
	defaultFilterShader = PxDefaultSimulationFilterShader;
	physicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, defaultAllocator, defaultErrorCallback);
	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *physicsFoundation, PxTolerancesScale());
	PxInitExtensions(*physics);
	physicsMaterial = physics->createMaterial(1, 1, 0);
	physicsCooker = PxCreateCooking(PX_PHYSICS_VERSION, *physicsFoundation, PxCookingParams(PxTolerancesScale()));
}


PxScene* PhysXScene::CreateDefaultScene()
{
	PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0, -9.8f, 0);
	sceneDesc.filterShader = filterShader; //!
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(8);
	PxScene* result = physics->createScene(sceneDesc);
	return result;
}

void PhysXScene::setupFiltering(PxRigidActor* actor, PxU32 filterGroup, PxU32 filterMask)
{
	PxFilterData filterData;
	filterData.word0 = filterGroup;
	filterData.word1 = filterMask;

	const PxU32 numShapes = actor->getNbShapes();
	PxShape** shapes = (PxShape**)_aligned_malloc(sizeof(PxShape*)*numShapes, 16);
	actor->getShapes(shapes, numShapes);

	for (PxU32 i = 0; i < numShapes; i++)
	{
		PxShape* shape = shapes[i];
		shape->setSimulationFilterData(filterData);
	}

	_aligned_free(shapes);

}

void PhysXScene::setShapeAsTrigger(PxRigidActor* actor)
{
	PxRigidStatic* staticActor = actor->is<PxRigidStatic>();
	assert(staticActor);

	const PxU32 numShapes = staticActor->getNbShapes();
	PxShape** shapes = (PxShape**)_aligned_malloc(sizeof(PxShape*)*numShapes, 16);
	staticActor->getShapes(shapes, numShapes);
	for (PxU32 i = 0; i < numShapes; ++i)
	{
		shapes[0]->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		shapes[0]->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}
	_aligned_free(shapes);
}



