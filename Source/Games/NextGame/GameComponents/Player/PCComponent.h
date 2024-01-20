#pragma once

#include "BehemothEngine.h"


class PCComponent : public ECS::Component
{
public:
	PCComponent() = default;

	PCComponent(
		Behemoth::KeyCode incPowerKC,
		Behemoth::KeyCode decPowerKC,
		Behemoth::KeyCode fireKC,
		Behemoth::KeyCode swapCamKC,
// 		Behemoth::ControllerCode incPowerCC,
// 		Behemoth::ControllerCode decPowerCC,
		Behemoth::ControllerCode fireCC,
		Behemoth::ControllerCode swapCamCC)
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

