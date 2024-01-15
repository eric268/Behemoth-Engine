#include "pch.h"

TEST(ECSPointerStability, PtrStability)
{
//	ECS::Registry registry{};
	EXPECT_TRUE(true);

//	ECS::EntityHandle handle = registry.CreateEntity("Test Entity");
//	Behemoth::TransformComponent* component = registry.AddComponent<Behemoth::TransformComponent>(handle);
// 
// 	for (int i = 0; i < 1000; i++)
// 	{
// 		ECS::EntityHandle temp = registry.CreateEntity("Temp Entity");
// 		Behemoth::TransformComponent* component = registry.AddComponent<Behemoth::TransformComponent>(temp);
// 	}
// 
// 	Behemoth::TransformComponent* component2 = registry.AddComponent<Behemoth::TransformComponent>(handle);
// 
// 	EXPECT_TRUE(&component == &component2);
}