#include "canpch.h"
#include "Scene.h"
#include "GameLayer.h"

Scene::Scene(float width, float heigth, float movementSpeed, GameLayer* gameLayer)
	: m_ObstacleCount(10)
	, m_Width(width)
	, m_Height(heigth)
	, m_MovementSpeed(movementSpeed)
	, m_ObstacleWidth(1.0f)
	, m_MinObstacleHeight(0.9f)
	, m_MaxObstacleHeight(1.3f)
	, m_MinSpaceWidth(3.0f)
	, m_MaxSpaceWidth(6.0f)
	, m_GameLayer(gameLayer)
{
	for (int i = 0; i < m_ObstacleCount; i++)
	{
		AddObstacle();
	}
	m_ObstacleTexture = Can::Texture2D::Create("assets/textures/Cactus2.png");
}

Scene::~Scene()
{
	m_Obstacles.clear();
}

void Scene::Update(float ts)
{
	m_MovementSpeed += ts * 0.02f;
	float offset = ts * m_MovementSpeed;
	for (int i = 0; i < m_ObstacleCount; i++)
	{
		m_Obstacles.at(i).x -= offset;
		m_Obstacles.at(i).x -= offset;
		if (m_Obstacles.at(i).x < -0.5f)
		{
			m_GameLayer->AddPoint();
			m_Obstacles.erase(m_Obstacles.begin());
			AddObstacle();
			i--;
		}
	}
}

void Scene::Draw(glm::vec3 offset)
{
	Can::DrawQuadParameters params = Can::DrawQuadParameters();
	
	params.Position = { 0.0f, 0.0f, -0.11111f };
	params.Size = { m_Width * 2, m_Height * 2 };
	params.TintColor = { 0.85f, 0.85f, 0.85f, 1.0f };
	Can::Renderer2D::DrawQuad(params);

	params.Position = { 0.0f, m_Height * 0.2f * 3.0f / 2.0f, -0.111f };
	params.Size = { m_Width, m_Height };
	params.TintColor = { 1.0f, 1.0f, 0.85f, 1.0f };
	Can::Renderer2D::DrawQuad(params);

	params.Position = { 0.0f, m_Height * 0.8f / 2.0f, -0.11f };
	params.Size = { m_Width, m_Height * 0.8f };
	params.TintColor = glm::vec4(1.0f);
	params.texture = m_GameLayer->m_SkyImage;
	Can::Renderer2D::DrawQuad(params);

	params.Position = { 0.0f, -m_Height * 0.3f / 2.0f, -0.1f };
	params.Size = { m_Width, m_Height * 0.3f };
	params.texture = m_GameLayer->m_GroundImage;
	Can::Renderer2D::DrawQuad(params);

	for (int i = 0; i < m_ObstacleCount; i++)
	{
		Obstacle ob = m_Obstacles.at(i);
		if (ob.x < m_Width - m_ObstacleWidth * 2)
		{
			params.Position = { ob.x + offset.x , ob.h / 2 + offset.y ,offset.z };
			params.Size = { ob.w * 1.2f, ob.h * 1.1f };
			if (ob.h > 1.0f)
			{
				params.texture = m_GameLayer->m_CactusImage1;
				Can::Renderer2D::DrawQuad(params);
			}
			else
			{
				params.texture = m_GameLayer->m_CactusImage2;
				Can::Renderer2D::DrawQuad(params);
			}
		}
	}
}

void Scene::AddObstacle()
{
	float x = m_Obstacles.size() > 0 ? m_Obstacles.back().x : 5.0f;

	float spacew = Can::Utility::Random::Float(m_MinSpaceWidth, m_MaxSpaceWidth);
	float r = Can::Utility::Random::Float(-1.0f, 1.0f);
	float height = r >= 0.0f ? m_MinObstacleHeight : m_MaxObstacleHeight;
	x += spacew + m_ObstacleWidth;

	m_Obstacles.push_back(Obstacle(x, height, m_ObstacleWidth, spacew));
}
