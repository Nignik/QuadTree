#pragma once

#include <vector>
#include <cassert>

#include "include.h"
#include "SpaceUtils.h"
#include "PositionComponent.h"

/*
* If a node doesn't have objects in the given division square, then the pointer to the child node of that square is nullptr
*/

class QuadTreeNode
{
public:
	QuadTreeNode(std::vector<Hori::Entity>& objects, Rect bounds, int level=0)
		: bounds(bounds),
		level(level)
	{
		if (objects.size() <= maxLevel || level >= maxLevel)
		{
			objects = objects;
			return;
		}

		auto& world = Hori::World::GetInstance();

		Rect nwRect{ bounds.x, bounds.y, bounds.w / 2, bounds.h / 2 };
		Rect neRect{ bounds.x + bounds.w / 2,bounds.y,bounds.w / 2,bounds.h / 2 };
		Rect swRect{ bounds.x,bounds.y + bounds.h / 2,bounds.w / 2,bounds.h / 2 };
		Rect seRect{ bounds.x + bounds.w / 2,bounds.y + bounds.h / 2,bounds.w / 2,bounds.h / 2 };

		std::vector<Hori::Entity> nwObjects, neObjects, swObjects, seObjects;
		for (auto& object : objects)
		{
			assert(world.HasComponents<PositionComponent>(object), "ERROR: Entity in QuadTree doesn't have a position component");
			auto position = world.GetComponent<PositionComponent>(object)->value;

			// Do this to use sdl function, can later be changed
			auto p = vecToPoint(position);
			if (PointInRect(&p, &nwRect)) nwObjects.push_back(object);
			else if (PointInRect(&p, &neRect)) neObjects.push_back(object);
			else if (PointInRect(&p, &swRect)) swObjects.push_back(object);
			else if (PointInRect(&p, &seRect)) seObjects.push_back(object);
		}

		float halfWidth = bounds.w / 2;
		float halfHeight = bounds.h / 2;
		if (nwObjects.size() > 0) nw = std::make_shared<QuadTreeNode>(nwObjects, nwRect, level + 1);
		if (neObjects.size() > 0) ne = std::make_shared<QuadTreeNode>(neObjects, neRect, level + 1);
		if (swObjects.size() > 0) sw = std::make_shared<QuadTreeNode>(swObjects, swRect, level + 1);
		if (seObjects.size() > 0) se = std::make_shared<QuadTreeNode>(seObjects, seRect, level + 1);
	}

	Rect bounds{};
	int level = 0;
	int maxLevel = 5;
	int maxObjects = 4;

	std::vector<Hori::Entity> objects{};
	std::shared_ptr<QuadTreeNode> nw{}, ne{}, sw{}, se{};
};