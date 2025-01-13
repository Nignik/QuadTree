#pragma once

#include <SDL3/SDL.h>
#include <World.h>
#include <vector>

#include "QuadTree.h"

class Scene
{
public:
	Scene();

	void Update(float dt, SDL_Renderer* renderer);

private:
	QuadTree m_quadTree;
	std::vector<Hori::Entity> m_pointEntities{};

	void Draw(SDL_Renderer* renderer);
	void DrawQuadTree(std::shared_ptr<QuadTreeNode> node, SDL_Renderer* renderer);
};