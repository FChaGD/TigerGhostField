#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMontage.h"
#include "EnemyPatternData.generated.h"

USTRUCT(BlueprintType)
struct FEnemyPatternData : public FTableRowBase
{
	GENERATED_BODY()

public:
	/** 패턴을 구분하는 Id 정보입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	int32 PatternID = 0;

	/** 몬스터를 구분하는 Id 정보입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	int32 EnemyID = 0;

	/** 전투 시작 직후 패턴에 부여되는 초기 재사용 대기시간 정보입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float InitialDelay = 0.f;

	/** 패턴 사용 후 부여되는 재사용 대기시간 정보입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float Cooldown = 0.f;

	/** 패턴 사용 후 다른 패턴을 사용할 수 없는 최소 대기시간 정보입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float MinNextPatternDelay = 0.f;

	/** 패턴 선택 우선순위 정보입니다. 수치가 낮을수록 먼저 선택됩니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	int32 Priority = 0;

	/** 동일 우선순위 패턴 중 선택 확률을 결정하는 빈도 비율 정보입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float Frequency = 1.f;

	/** 패턴 실행이 가능한 플레이어와의 최대 거리 정보입니다. (cm 단위) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	float PatternRange = 100.f;

	/** 패턴 실행 시 재생할 AnimMontage 정보입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	TObjectPtr<UAnimMontage> PatternMontage = nullptr;
};