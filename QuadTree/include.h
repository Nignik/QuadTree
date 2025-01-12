#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <functional>

using Point = SDL_FPoint;
using Rect = SDL_FRect;
const auto PointInRect = SDL_PointInRectFloat;