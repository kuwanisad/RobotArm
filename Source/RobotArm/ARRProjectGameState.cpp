// Fill out your copyright notice in the Description page of Project Settings.


#include "ARRProjectGameState.h"
#include "Coin.h"
#include "RobotArmActor.h"
#include "GameFramework/Pawn.h"
#include "ARPawn.h"
#include "GameFramework/PlayerController.h"
#include "RobotArmDestination.h"
#include "Kismet/GameplayStatics.h"


void AARRProjectGameState::AddRobotArmDestination()
{
	if (IsValid(RobotArmDestinationClass) == false)
	{
		GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Red, TEXT("CoinClass가 nullptr입니다."));
		return;
	}

	FVector FinalPosition = SpawnReachablePosition();

	FActorSpawnParameters Param;
	Param.Owner = this;
	GetWorld()->SpawnActor<ARobotArmDestination>(RobotArmDestinationClass, FinalPosition, FRotator::ZeroRotator, Param);
}

ACoin* AARRProjectGameState::AddScoreAndSpawnCoin(int32 _Score)
{
	if (IsValid(CoinClass) == false)
	{
		GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Red, TEXT("CoinClass가 nullptr입니다."));
		return nullptr;
	}

	FVector FinalPosition = SpawnReachablePosition();

	Score += _Score;
	OnScoreChanged.Broadcast(Score);

	FActorSpawnParameters Param;
	Param.Owner = this;
	return GetWorld()->SpawnActor<ACoin>(CoinClass, FinalPosition, FRotator::ZeroRotator, Param);
}

FVector AARRProjectGameState::SpawnReachablePosition()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARobotArmActor::StaticClass(), FoundActors);

	ARobotArmActor* RobotArm = nullptr;
	for (AActor* Actor : FoundActors)
	{
		RobotArm = Cast<ARobotArmActor>(Actor);
		if (RobotArm)
		{
			break;
		}
	}

	if (RobotArm == nullptr)
	{
		return FVector::ZeroVector;
	}

	FVector Direction = { 1, 0, 0 };
	FRotator Angle;
	Angle.Pitch = FMath::FRandRange(0, 360.f);
	Angle.Roll = FMath::FRandRange(0, 360.f);
	Angle.Yaw = FMath::FRandRange(0, 360.f);

	Direction = Angle.RotateVector(Direction);
	//Direction *= SpawnMinRadius + FMath::FRandRange(0, SpawnMaxRadius - SpawnMinRadius);
	Direction *= 290;
	Direction.Y = FMath::Max(100, Direction.Y);

	//FVector FinalPosition;

	if (RobotArm == nullptr)
	{
		return FVector(0, 0, 0);
	}

	//RobotArm->GetReachablePosition(RobotArm->GetActorLocation() + Direction, FinalPosition);

	//return FinalPosition;

	Direction.Z += 150;

	return RobotArm->GetActorLocation() + Direction;
}

int32 AARRProjectGameState::GetScore()
{
	return Score;
}
