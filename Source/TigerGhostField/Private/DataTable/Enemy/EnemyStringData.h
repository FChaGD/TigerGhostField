#pragma once

#include "CoreMinimal.h"
#include "EnemyStringData.generated.h"

USTRUCT()
struct FEnemyStringData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	/** 몬스터를 구분하는 ID 정보입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnemyID;
	
	/** 몬스터의 이름에 대한 정보입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EnemyName;
	
	/** 몬스터의 플레이버 텍스트 정보입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EnemyFlavorText;
	
	/** 몬스터의 시나리오 텍스트 정보입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EnemyScenarioText;
	
	/** 몬스터의 인게임 관련 설명 텍스트 정보입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EnemyInfoText;
	
};
