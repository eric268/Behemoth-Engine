#pragma once

#include "World/Scene.h"

class MainScene : public Behemoth::Scene
{
public:
	void Init() override;
	void Update(const float deltaTime) override;
	void Shutdown() override;

private:

};

