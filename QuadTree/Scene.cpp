#include "Scene.h"

#include <random>


Scene::Scene()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis_x(0.f, 1280.f);
	std::uniform_real_distribution<float> dis_y(0.f, 720.f);

	auto& world = Hori::World::GetInstance();

	int numOfPoints = 30;
	m_pointEntities.reserve(numOfPoints);
	std::vector<Point> points;
	points.reserve(numOfPoints);
	for (int i = 0; i < numOfPoints; i++)
	{
		auto entity = world.CreateEntity();
		points.emplace_back(dis_x(gen), dis_y(gen));
		world.AddComponents<>(entity, points.back());
		m_pointEntities.push_back(entity);
	}


	m_quadTree = QuadTree(points, Rect{ 0.f, 0.f, 1280.f, 720.f });
}

void Scene::Update(float dt, SDL_Renderer* renderer)
{
	Draw(renderer);
}

void Scene::Draw(SDL_Renderer* renderer)
{
	auto& world = Hori::World::GetInstance();

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	for (auto& pEntity : m_pointEntities)
	{
		auto p = world.GetComponent<Point>(pEntity);

		float scale = 5.f;
		Rect r{ p->x - scale, p->y - scale, scale, scale };
		SDL_RenderFillRect(renderer, &r);
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	DrawQuadTree(m_quadTree.root, renderer);
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
