#pragma once

#include "BehemothEngine.h"

using namespace Behemoth;
struct PCComponent : public ECS::Component
{
public:
	PCComponent() = default;

	PCComponent(
		KeyCode incPowerKC,
		KeyCode decPowerKC,
		KeyCode fireKC,
		KeyCode aimLeftKC,
		KeyCode aimRightKC,
		KeyCode aimUpKC,
		KeyCode aimDownKC,
		KeyCode lookLeftKC,
		KeyCode lookRightKC,
		KeyCode lookUpKC,
		KeyCode lookDownKC,
		ControllerCode fireCC)
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
		KeyCode zoomInKC,
		KeyCode zoomOutKC,
		KeyCode rotateLeftKC,
		KeyCode rotateRightKC,
		KeyCode moveUpKC,
		KeyCode moveDownKC,
		ControllerCode moveUpCC,
		ControllerCode moveDownCC)
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

	KeyCode zoomInKC;
	KeyCode zoomOutKC;
	KeyCode rotateLeftKC;
	KeyCode rotateRightKC;
	KeyCode moveUpKC;
	KeyCode moveDownKC;

	ControllerCode moveUpCC;
	ControllerCode moveDownCC;
};

