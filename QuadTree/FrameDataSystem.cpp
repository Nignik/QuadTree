#include "FrameDataSystem.h"
#include "FrameDataComponent.h"

#include <cassert>

void FrameDataSystem::Update(float dt)
{
	auto& world = Hori::World::GetInstance();

	auto frameData = world.GetSingletonComponent<FrameDataComponent>();
	assert(frameData != nullptr, "ERROR: There is not frame data component");

	float ticks = SDL_GetTicks();
	frameData->dt = ticks - frameData->lastTick;
	frameData->lastTick = ticks;
}
