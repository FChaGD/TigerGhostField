// Author: ju wonung
#include "Component/UI/EnemyUIComponent.h"
#include "Widget/UserWidgetBase.h"

void UEnemyUIComponent::RegisterEnemyDrawnWidget(UUserWidgetBase* InWidgetToRegister)
{
	EnemyDrawnWidgets.Add(InWidgetToRegister);
}

void UEnemyUIComponent::RemoveEnemyDrawnWidgetsIfAny()
{
	if (EnemyDrawnWidgets.IsEmpty()) return;

	// All Delete Widget from Array
	for (UUserWidgetBase* DrawnWidget : EnemyDrawnWidgets)
	{
		if (DrawnWidget)
		{
			DrawnWidget->RemoveFromParent();
		}
	}

	EnemyDrawnWidgets.Empty();
}