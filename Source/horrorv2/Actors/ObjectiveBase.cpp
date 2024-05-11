// Fill out your copyright notice in the Description page of Project Settings.


#include "../Actors/ObjectiveBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AObjectiveBase::AObjectiveBase()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(StaticMesh);
	Collision->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	Collision->SetBoxExtent(FVector(5.f, 5.f, 5.f));
	Collision->SetCollisionProfileName(TEXT("Trigger"));
	
}

// Called when the game starts or when spawned
void AObjectiveBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObjectiveBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

