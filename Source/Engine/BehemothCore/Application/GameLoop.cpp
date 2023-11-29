
#include <windows.h> 
#include <iostream>
#include "ECS/ECSCore.h"
#include "Render/Primitives.h"
#include "Misc/Log.h"

//stl
#include <iostream>

//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------

BehemothEngine::Primitives* p;

using namespace ECS;
void Init()
{
	Registry registry;

	Entity e1 = registry.CreateEntity("Entity 1");
	Entity e2 = registry.CreateEntity("Entity 2");
	Entity e3 = registry.CreateEntity("Entity 3");
	Entity e4 = registry.CreateEntity("Entity 4");
	Entity e5 = registry.CreateEntity("Entity 5");

	registry.AddComponent<HealthComponent>(e1,  100 );
	registry.AddComponent<HealthComponent>(e2,  90 );
	registry.AddComponent<HealthComponent>(e3,  0 );

	registry.AddComponent<RenderComponent>(e1);
	registry.AddComponent<RenderComponent>(e2);
	registry.AddComponent<RenderComponent>(e3);
	registry.AddComponent<RenderComponent>(e4);
	registry.AddComponent<RenderComponent>(e5);

	registry.AddComponent<MovementComponent>(e1);
	registry.AddComponent<MovementComponent>(e2);

	registry.AddComponent<PhysicsComponent>(e2);

	auto group = registry.Group<HealthComponent, RenderComponent, MovementComponent, PhysicsComponent>();
	
	auto comp = registry.GetComponent<HealthComponent>();

	std::vector<Math::Vector3> verticies(3);

	float m_width = 200;
	float m_height = 200;

	verticies[0].x = -(m_width / 2.0f);
	verticies[0].y = -(m_height / 2.0f);
	verticies[1].x = m_width / 2.0f;
	verticies[1].y = -(m_height / 2.0f);
	verticies[2].x = m_width / 2.0f;
	verticies[2].y = m_height / 2.0f;

	p = new BehemothEngine::Primitives();
	p->SetVerticies(verticies);
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{
	p->sprite->Draw();
}
//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
}
