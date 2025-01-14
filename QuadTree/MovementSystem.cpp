#include "MovementSystem.h"

#include "VelocityComponent.h"
#include "PositionComponent.h"
#include "WindowDataComponent.h"
#include "include.h"

void MovementSystem::Update(float dt)
{
	auto& world = Hori::World::GetInstance();
	auto windowData = world.GetSingletonComponent<WindowDataComponent>();

	for (auto entity : world.GetEntitiesWithComponents<VelocityComponent>())
	{
		auto& velocity = world.GetComponent<VelocityComponent>(entity)->value;
		auto& position = world.GetComponent<PositionComponent>(entity)->value;

		position += velocity * dt;
		if (position.x > windowData->windowWidth || position.x < 0)
			velocity.x = -velocity.x;
			
		if (position.y > windowData->windowHeight || position.y < 0)
			velocity.y = -velocity.y;

		position.x = glm::clamp(position.x, 0.f, (float)windowData->windowWidth);
		position.y = glm::clamp(position.y, 0.f, (float)windowData->windowHeight);
	}
}
