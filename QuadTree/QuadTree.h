#pragma once

#include <vector>

#include "include.h"
#include "SpaceUtils.h"

class QuadTreeNode
{
public:
	QuadTreeNode(std::vector<Vec2>& points, Rect bounds)
		: bounds(bounds)
	{
		if (points.size() <= 1 || bounds.w <= 1 || bounds.h <= 1)
			return;
		
		std::vector<Vec2> nwPoints, nePoints, swPoints, sePoints;
		Rect nwRect{ bounds.x, bounds.y, bounds.w / 2, bounds.h / 2};
		Rect neRect{bounds.x + bounds.w / 2,bounds.y,bounds.w / 2,bounds.h / 2};
		Rect swRect{bounds.x,bounds.y + bounds.h / 2,bounds.w / 2,bounds.h / 2};
		Rect seRect{bounds.x + bounds.w / 2,bounds.y + bounds.h / 2,bounds.w / 2,bounds.h / 2};

		for (auto& p : points)
		{
			if (PointInRect(&p, &nwRect)) nwPoints.push_back(p);
			else if (PointInRect(&p, &neRect)) nePoints.push_back(p);
			else if (PointInRect(&p, &swRect)) swPoints.push_back(p);
			else if (PointInRect(&p, &seRect)) sePoints.push_back(p);
		}

		if (nwPoints.size() > 0) nw = std::make_shared<QuadTreeNode>(nwPoints, nwRect);
		if (nePoints.size() > 0) ne = std::make_shared<QuadTreeNode>(nePoints, neRect);
		if (swPoints.size() > 0) sw = std::make_shared<QuadTreeNode>(swPoints, swRect);
		if (sePoints.size() > 0) se = std::make_shared<QuadTreeNode>(sePoints, seRect);
	}

	Rect bounds;
	std::shared_ptr<QuadTreeNode> nw, ne, sw, se;
};

class QuadTree
{
public:
	QuadTree()
	{
	}

	QuadTree(std::vector<Vec2>& points, Rect bounds)
	{
		if (points.size() <= 0)
			return;

		root = std::make_shared<QuadTreeNode>(points, bounds);
	}

	std::shared_ptr<QuadTreeNode> root = nullptr;
};