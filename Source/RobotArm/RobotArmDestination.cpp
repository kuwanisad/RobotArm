// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotArmDestination.h"
#include "Components/StaticMeshComponent.h"
#include "ARRProjectGameState.h"
#include "RobotArmActor.h"

// Sets default values
ARobotArmDestination::ARobotArmDestination()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(FName(TEXT("overlapAll")));


}

// Called when the game starts or when spawned
void ARobotArmDestination::BeginPlay()
{
	Super::BeginPlay();
	
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ARobotArmDestination::OnOverlap);
}

void ARobotArmDestination::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Mesh->AddWorldRotation(FRotator(0, RotationSpeed * DeltaTime, 0));
}

void ARobotArmDestination::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsActive == false)
	{
		return;
	}

	if (IsValid(OtherActor))
	{
		if (ARobotArmActor* RobotArm = Cast<ARobotArmActor>(OtherActor))
		{
			if (AARRProjectGameState* GS = Cast<AARRProjectGameState>(GetWorld()->GetGameState()))
			{
				IsActive = false;
				GS->AddScoreAndSpawnCoin(CoinScore);
				RobotArm->RemoveCoin();
				Destroy();
			}
		}
	}
}

