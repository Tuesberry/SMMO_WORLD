// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Engine/DataTable.h>
#include "CharacterData.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FCharacterData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData")
		FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData")
		USkeletalMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData")
		TSubclassOf<UAnimInstance> AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData")
		UAnimMontage* WavingMontage;
};
