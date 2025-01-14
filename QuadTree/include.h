#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <World.h>
#include <functional>

#include <glm/glm.hpp>

using Vec2 = glm::vec2;
using Point = SDL_FPoint;
using Rect = SDL_FRect;
const auto PointInRect = SDL_PointInRectFloat;

inline Vec2 pointToVec(Point point)
{
	return Vec2{ point.x, point.y };
}

inline Point vecToPoint(Vec2 vec)
{
	return Point{ vec.x, vec.y };
}