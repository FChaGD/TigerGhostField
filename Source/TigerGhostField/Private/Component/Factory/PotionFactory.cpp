// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Factory/PotionFactory.h"
#include "Character/Potion/PotionBase.h"
#include "Character/Potion/HumanPotion.h"
#include "Character/Potion/WraithPotion.h"
#include "Character/Potion/YokaiPotion.h"

// Potion 생성함수
UPotionBase* UPotionFactory::CreatePotion(int test)
{
	TSubclassOf<UPotionBase> PotionClass = nullptr;

	UE_LOG(LogTemp, Warning, TEXT("Create Potion"));
	switch (test)
	{
	case 0:
		PotionClass = PotionClassSet.HumanPotionClass;
		return NewObject<UPotionBase>(this, PotionClass);
	case 1:
		PotionClass = PotionClassSet.HumanPotionClass;
		return NewObject<UPotionBase>(this, PotionClass);
	case 2:
		PotionClass = PotionClassSet.HumanPotionClass;
		return NewObject<UPotionBase>(this, PotionClass);
	default:
		break;
	}
	
	return nullptr;
}
