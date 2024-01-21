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
		KeyCode swapCamKC,
// 		Behemoth::ControllerCode incPowerCC,
// 		Behemoth::ControllerCode decPowerCC,
		ControllerCode fireCC,
		ControllerCode swapCamCC)
		:
		increasePowerKC(incPowerKC),
		decreasePowerKC(decPowerKC),
		fireProjectileKC(fireKC),
		swapCameraViewKC(swapCamKC),
// 		increasePowerCC(incPowerCC),
// 		decreasePowerCC(decPowerCC),
		fireProjectileCC(fireCC),
		swapCameraViewCC(swapCamCC)
	{}
	
	Behemoth::KeyCode increasePowerKC;
	Behemoth::KeyCode decreasePowerKC;
	Behemoth::KeyCode fireProjectileKC;
	Behemoth::KeyCode swapCameraViewKC;

// 	Behemoth::ControllerCode increasePowerCC;
// 	Behemoth::ControllerCode decreasePowerCC;
	Behemoth::ControllerCode fireProjectileCC;
	Behemoth::ControllerCode swapCameraViewCC;
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

