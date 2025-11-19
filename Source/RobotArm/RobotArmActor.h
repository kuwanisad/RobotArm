// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RobotArmActor.generated.h"

UStaticMeshComponent;

UCLASS()
class ROBOTARM_API ARobotArmActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARobotArmActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UStaticMeshComponent>> Parts;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UStaticMeshComponent>> Joints;

	UPROPERTY(EditAnywhere)
	bool Temp = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*
	반환값은 계산 수행 여부
	*/
	void GetReachablePosition(FVector OriginalPosition, FVector& ReachablePosition);
};
