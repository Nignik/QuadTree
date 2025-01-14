#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <memory>

#include "include.h"
#include "Scene.h"
#include "WindowDataComponent.h"
#include "MovementSystem.h"
#include "FrameDataSystem.h"
#include "FrameDataComponent.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static Scene scene;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    Hori::World& world = Hori::World::GetInstance();
    world.AddSingletonComponent(WindowDataComponent());
    world.AddSingletonComponent(FrameDataComponent());
    auto windowData = world.GetSingletonComponent<WindowDataComponent>();

    if (!SDL_CreateWindowAndRenderer("QuadTree", windowData->windowWidth, windowData->windowHeight, 0, &window, &renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    world.AddSystem<MovementSystem>(MovementSystem());
    world.AddSystem<FrameDataSystem>(FrameDataSystem());

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

SDL_AppResult SDL_AppIterate(void* appstate)
{
    const double now = ((double)SDL_GetTicks()) / 1000.0;

    SDL_SetRenderDrawColorFloat(renderer, 0.0f, 0.0f, 0.2f, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(renderer);

    Hori::World& world = Hori::World::GetInstance();
    // Ummmm... thats kidna messed up, i cant pass that dt, as frame data system needs to be updated before. The current frame will use dt from the previous frame (possibly bad)
    world.UpdateSystems(world.GetSingletonComponent<FrameDataComponent>()->dt);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    scene.Update(0.f, renderer);

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    
}
