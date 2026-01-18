// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attack/AttackBase.h"

#include "DebugHelper.h"

UAttackBase::UAttackBase()
{
}

UAttackBase::~UAttackBase()
{
}

void UAttackBase::Initialize(ABaseCharacter* InOwnerCharacter)
{
	OwnerCharacter = InOwnerCharacter;
}

void UAttackBase::Execute()
{
}

ABaseCharacter* UAttackBase::GetOwnerCharacter() const
{
	return OwnerCharacter;
}
