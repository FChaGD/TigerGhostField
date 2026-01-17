// Author: ju wonung
#include "Interface/PawnUIInterface.h"

// Add default functionality here for any IPawnUIInterface functions that are not pure virtual.
UPlayerUIComponent* IPawnUIInterface::GetPlayerUIComponent()
{
	return nullptr;
}

UEnemyUIComponent* IPawnUIInterface::GetEnemyUIComponent()
{
	return nullptr;
}
