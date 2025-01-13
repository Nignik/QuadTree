#include "MovementSystem.h"

#include "VelocityComponent.h"

void MovementSystem::Update(float dt)
{
	auto& world = Hori::World::GetInstance();

	for (auto entity : world.GetEntitiesWithComponents<VelocityComponent>())
	{
		auto velocity = world.GetComponent<VelocityComponent>(entity);


	}
}
