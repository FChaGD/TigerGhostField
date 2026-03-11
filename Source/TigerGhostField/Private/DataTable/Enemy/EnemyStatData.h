#pragma once

#include "CoreMinimal.h"
#include "EnemyStatData.generated.h"

UENUM()
enum class EEnemyType : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Elite  UMETA(DisplayName = "Elite"),
	Boss   UMETA(DisplayName = "Boss")
};

USTRUCT()
struct FEnemyStatData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	/** 몬스터를 구분하는 ID 정보입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnemyID;
	
	/** 몬스터의 공격력 정보입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackPower;
	
	/** 몬스터의 생명력 최대치 정보입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP;
	
	/** 몬스터의 지구력 최대치 정보입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxStamina;
	
	/** 몬스터의 정신력 최대치 정보입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxMentality;
	
	/** 몬스터의 영력 최대치 정보입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSpiritualPower;
	
	/** 몬스터의 분류 종류 정보입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEnemyType EnemyType;
	
	/** 몬스터의 발악 패턴 유무 정보입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bHasStrugglePattern : 1;
	
};
