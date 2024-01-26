#pragma once

#include "BehemothEngine.h"

struct PCComponent : public ECS::Component
{
public:
	PCComponent() = default;

	PCComponent(
		Behemoth::KeyCode incPowerKC,
		Behemoth::KeyCode decPowerKC,
		Behemoth::KeyCode fireKC,
		Behemoth::KeyCode aimLeftKC,
		Behemoth::KeyCode aimRightKC,
		Behemoth::KeyCode aimUpKC,
		Behemoth::KeyCode aimDownKC,
		Behemoth::KeyCode lookLeftKC,
		Behemoth::KeyCode lookRightKC,
		Behemoth::KeyCode lookUpKC,
		Behemoth::KeyCode lookDownKC,
		Behemoth::ControllerCode fireCC)
		:
		increasePowerKC(incPowerKC),
		decreasePowerKC(decPowerKC),
		fireProjectileKC(fireKC),
		aimLeftKC(aimLeftKC),
		aimRightKC(aimRightKC),
		aimUpKC(aimUpKC),
		aimDownKC(aimDownKC),
		lookLeftKC(lookLeftKC),
		lookRightKC(lookRightKC),
		lookUpKC(lookUpKC),
		lookDownKC(lookDownKC),
		fireProjectileCC(fireCC)
	{}
	
	Behemoth::KeyCode increasePowerKC;
	Behemoth::KeyCode decreasePowerKC;
	Behemoth::KeyCode fireProjectileKC;
	Behemoth::KeyCode aimLeftKC;
	Behemoth::KeyCode aimRightKC;
	Behemoth::KeyCode aimUpKC;
	Behemoth::KeyCode aimDownKC;
	Behemoth::KeyCode lookLeftKC;
	Behemoth::KeyCode lookRightKC;
	Behemoth::KeyCode lookUpKC;
	Behemoth::KeyCode lookDownKC;

	Behemoth::ControllerCode fireProjectileCC;
};

struct ViewControllerComponent : public ECS::Component
{
	ViewControllerComponent() = default;

	ViewControllerComponent(
		bool isActive,
		Behemoth::KeyCode zoomInKC,
		Behemoth::KeyCode zoomOutKC,
		Behemoth::KeyCode rotateLeftKC,
		Behemoth::KeyCode rotateRightKC,
		Behemoth::KeyCode moveUpKC,
		Behemoth::KeyCode moveDownKC,
		Behemoth::ControllerCode moveUpCC,
		Behemoth::ControllerCode moveDownCC)
		:
		zoomInKC(zoomInKC),
		zoomOutKC(zoomOutKC),
		rotateLeftKC(rotateLeftKC),
		rotateRightKC(rotateRightKC),
		moveUpKC(moveUpKC),
		moveDownKC(moveDownKC),
		moveUpCC(moveUpCC),
		moveDownCC(moveDownCC)
	{}

	Behemoth::KeyCode zoomInKC;
	Behemoth::KeyCode zoomOutKC;
	Behemoth::KeyCode rotateLeftKC;
	Behemoth::KeyCode rotateRightKC;
	Behemoth::KeyCode moveUpKC;
	Behemoth::KeyCode moveDownKC;

	Behemoth::ControllerCode moveUpCC;
	Behemoth::ControllerCode moveDownCC;
};

