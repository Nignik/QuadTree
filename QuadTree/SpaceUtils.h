#pragma once

#include "include.h"
#include <cmath>

inline float distance(Vec2 p, Rect r)
{
	float dist = 0;
	if (p.x < r.x + r.w)
		dist += std::powf(r.x + r.w - p.x, 2);
	else if (p.x > r.x)
		dist += std::powf(p.x - r.x, 2);

	if (p.y < r.y + r.h)
		dist += std::powf(r.y + r.h - p.y, 2);
	else if (p.y > r.y)
		dist += std::powf(p.y - r.y, 2);

	return std::sqrtf(dist);
}