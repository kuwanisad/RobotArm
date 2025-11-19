// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotArmActor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Coin.h"

// Sets default values
ARobotArmActor::ARobotArmActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//SetActorTickEnabled(false);

	Parts.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Part1")));
	Parts.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Part2")));
	Parts.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Part3")));
	Parts.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Part4")));

	Joints.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Joint1")));
	Joints.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Joint2")));
	Joints.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Joint3")));

	SetRootComponent(Parts[0]);

	for (int i = 1; i < Parts.Num(); ++i)
	{
		Parts[i]->SetupAttachment(Joints[i - 1]);
	}

	for (int i = 0; i < Joints.Num(); ++i)
	{
		Joints[i]->SetupAttachment(Parts[i]);
	}
}

// Called when the game starts or when spawned
void ARobotArmActor::BeginPlay()
{
	Super::BeginPlay();
	
}


void ARobotArmActor::GetReachablePosition(FVector OriginalPosition, FVector& ReachablePosition)
{
	TArray<FTransform> OriginalTransformPartsArray;
	TArray<FTransform> OriginalTransformJointsArray;

	for (auto& Part : Parts)
	{
		OriginalTransformPartsArray.Add(Part->GetComponentTransform());
	}

	for (auto& Joint : Joints)
	{
		OriginalTransformJointsArray.Add(Joint->GetComponentTransform());
	}

	FVector TargetPos = OriginalPosition;

	float CurrentDistanceDelta = FVector::DistSquared(TargetPos, Parts.Last()->GetComponentLocation());
	float LastDistanceDelta = CurrentDistanceDelta + 1;

	while (CurrentDistanceDelta < LastDistanceDelta)
	{
		for (int Index = Joints.Num() - 1; Index >= 0; --Index)
		{
			USceneComponent* Parent = Joints[Index]->GetAttachParent();
			FVector PlaneNormal = FVector::CrossProduct(Parent->GetForwardVector(), Parent->GetRightVector());
			FPlane Plane(Joints[Index]->GetComponentLocation(), PlaneNormal);
			FVector ProjectedLocation = FVector::PointPlaneProject(TargetPos, Plane);
			FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(Joints[Index]->GetComponentLocation(), ProjectedLocation);

			Joints[Index]->SetWorldRotation(NewRotation);
			NewRotation = Joints[Index]->GetRelativeRotation();
			NewRotation.Pitch = 0;
			NewRotation.Roll = 0;
			Joints[Index]->SetRelativeRotation(NewRotation);

			LastDistanceDelta = CurrentDistanceDelta;
			CurrentDistanceDelta = FVector::DistSquared(TargetPos, Parts.Last()->GetComponentLocation());
		}
	}

	ReachablePosition = Parts.Last()->GetComponentLocation();

	Parts[0]->SetWorldTransform(OriginalTransformPartsArray[0]);
	
	for (int i = 1; i < OriginalTransformPartsArray.Num(); ++i)
	{
		Joints[i - 1]->SetWorldTransform(OriginalTransformJointsArray[i - 1]);
		Parts[i]->SetWorldTransform(OriginalTransformPartsArray[i]);
	}


}

void ARobotArmActor::GrabCoin(ACoin* _Coin)
{
	if (IsValid(_Coin) == false)
	{
		return;
	}

	if (IsValid(Coin))
	{
		return;
	}

	RemoveCoin();

	Coin = _Coin;
	Coin->AttachToComponent(Parts.Last(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ARobotArmActor::RemoveCoin()
{
	if (IsValid(Coin))
	{
		Coin->Destroy();
	}
}

// Called every frame
void ARobotArmActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

