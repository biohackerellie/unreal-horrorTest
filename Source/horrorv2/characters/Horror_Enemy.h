// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "Horror_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class HORRORV2_API AHorror_Enemy : public ATP_ThirdPersonCharacter
{
	GENERATED_BODY()

UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
USkeletalMeshComponent* MeshComponent;
	
public:
	AHorror_Enemy();

protected:
	virtual void BeginPlay();
};
