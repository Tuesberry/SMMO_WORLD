// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SMMOCharacterBase.generated.h"

UCLASS()
class SMMO_API ASMMOCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASMMOCharacterBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
		bool JumpButtonDown;

	void UpdateCharacterMesh(int TypeNum);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AnimationMontage)
		UAnimMontage* WavingMontage;

protected:
	void Jump();
	void StopJumping();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
