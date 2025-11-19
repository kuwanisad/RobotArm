// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ARRProjectGameState.generated.h"

class ACoin;
class ARobotArmDestination;
class ARobotArmActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, CurrentScore);


/**
 * 
 */
UCLASS()
class ROBOTARM_API AARRProjectGameState : public AGameStateBase
{
	GENERATED_BODY()
	
protected:
	int32 Score = 0;

	UPROPERTY(BlueprintAssignable)
	FOnScoreChanged OnScoreChanged;

	UPROPERTY(EditAnywhere)
	int32 SpawnAmount;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACoin> CoinClass;

public:

	UFUNCTION(BlueprintCallable)
	void AddRobotArmDestination();

	UFUNCTION(BlueprintCallable)
	ACoin* AddScoreAndSpawnCoin(int32 _Score);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARobotArmDestination> RobotArmDestinationClass;

	UFUNCTION(BlueprintCallable)
	FVector SpawnReachablePosition();

	UFUNCTION(BlueprintCallable)
	int32 GetScore();
};
