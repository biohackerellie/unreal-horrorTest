// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectiveBase.generated.h"


class UStaticMeshComponent;
class UBoxComponent;

UCLASS()

class HORRORV2_API AObjectiveBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Mesh)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category=Mesh)
	UBoxComponent* Collision;
	
public:	
	// Sets default values for this actor's properties
	AObjectiveBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
