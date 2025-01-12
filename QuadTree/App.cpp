#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <memory>

#include "include.h"
#include "QuadTree.h"

constexpr int screenWidth = 1280;
constexpr int screenHeight = 720;

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

std::unique_ptr<QuadTree> tree = nullptr;
std::vector<Point> points;
std::vector<std::vector<int>> colors = {
	{0, 255, 0},    // Bright Green
	{255, 255, 0},  // Yellow
	{0, 255, 255},  // Cyan
	{255, 0, 255},  // Magenta
	{0, 128, 0},    // Dark Green
	{128, 128, 0},  // Olive
	{128, 0, 128},  // Purple
	{0, 128, 128},  // Teal
};

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("QuadTree", screenWidth, screenHeight, 0, &window, &renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    for (int i = 0; i < screenWidth; i += 40)
    {
        for (int j = 0; j < screenHeight; j += 40)
        {
            points.emplace_back(Point{ (float)i, (float)j });
        }
    }
    tree = std::make_unique<QuadTree>(points, Rect{ 0.f, 0.f, (float)screenWidth, (float)screenHeight });

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

inline void drawQuadTree(std::shared_ptr<QuadTreeNode> node, int colorIdx)
{
	if (node == nullptr)
		return;

    SDL_SetRenderDrawColor(renderer, colors[colorIdx][0], colors[colorIdx][1], colors[colorIdx][2], SDL_ALPHA_OPAQUE);
	SDL_RenderRect(renderer, &node->bounds);

    if (colorIdx == colors.size() - 1)
        colorIdx--;

	drawQuadTree(node->nw, colorIdx + 1);
    drawQuadTree(node->ne, colorIdx + 1);
    drawQuadTree(node->sw, colorIdx + 1);
    drawQuadTree(node->se, colorIdx + 1);
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    const double now = ((double)SDL_GetTicks()) / 1000.0;

    SDL_SetRenderDrawColorFloat(renderer, 0.0f, 0.0f, 0.2f, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    drawQuadTree(tree->root, 0);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    for (auto& p : points)
    {
        float scale = 5.f;
        Rect r{ p.x - scale, p.y - scale, scale, scale };
        SDL_RenderFillRect(renderer, &r);
    }

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    
}
