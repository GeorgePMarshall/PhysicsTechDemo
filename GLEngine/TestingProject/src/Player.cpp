#include "Player.h"

void Player::Initialize()
{
	rigidbody.Initialize();
	rigidbody.setPosition(&position);
	rigidbody.AddSphereCollider(vec3(0), 1.0f);
}

void Player::Update()
{

}

void Player::Draw()
{
	Gizmos::addSphere(position, 1.0f, 32, 32, glm::vec4(1));
}