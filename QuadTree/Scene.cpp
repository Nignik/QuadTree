#include "Scene.h"

#include "VelocityComponent.h"
#include "PositionComponent.h"
#include "MovementSystem.h"
#include "WindowDataComponent.h"

#include <random>

Scene::Scene()
{
	Hori::World& world = Hori::World::GetInstance();
	auto windowData = world.GetSingletonComponent<WindowDataComponent>();

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis_x(0.f, (float)windowData->windowWidth);
	std::uniform_real_distribution<float> dis_y(0.f, (float)windowData->windowHeight);
	std::uniform_real_distribution<float> dis_norm(0.f, 1.f);

	int numOfPoints = 100;
	m_pointEntities.reserve(numOfPoints);
	std::vector<Vec2> points;
	points.reserve(numOfPoints);
	for (int i = 0; i < numOfPoints; i++)
	{
		auto entity = world.CreateEntity();
		points.emplace_back(dis_x(gen), dis_y(gen));
		world.AddComponents(entity, PositionComponent{ points.back() });
		world.AddComponents(entity, VelocityComponent{ Vec2{dis_norm(gen), dis_norm(gen)} / 3.f });
		m_pointEntities.push_back(entity);
	}

	m_quadTree = std::make_shared<QuadTreeNode>(m_pointEntities, Rect{ 0.f, 0.f, (float)windowData->windowWidth, (float)windowData->windowHeight});
}

void Scene::Update(float dt, SDL_Renderer* renderer)
{
	Hori::World& world = Hori::World::GetInstance();
	auto windowData = world.GetSingletonComponent<WindowDataComponent>();

	m_quadTree = std::make_shared<QuadTreeNode>(m_pointEntities, Rect{ 0.f, 0.f, (float)windowData->windowWidth, (float)windowData->windowHeight });
	Draw(renderer);
}

void Scene::Draw(SDL_Renderer* renderer)
{
	Hori::World& world = Hori::World::GetInstance();

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	for (auto& pEntity : m_pointEntities)
	{
		auto p = world.GetComponent<PositionComponent>(pEntity)->value;

		float scale = 5.f;
		Rect r{ p.x - scale, p.y - scale, scale, scale };
		SDL_RenderFillRect(renderer, &r);
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	DrawQuadTree(m_quadTree, renderer);
}

void Scene::DrawQuadTree(std::shared_ptr<QuadTreeNode> node, SDL_Renderer* renderer)
{
	if (node == nullptr)
		return;

	SDL_RenderRect(renderer, &node->bounds);

	DrawQuadTree(node->nw, renderer);
	DrawQuadTree(node->ne, renderer);
	DrawQuadTree(node->sw, renderer);
	DrawQuadTree(node->se, renderer);
}
