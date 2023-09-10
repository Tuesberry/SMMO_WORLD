// Fill out your copyright notice in the Description page of Project Settings.

#include "SMMOCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "SMMOGameInstance.h"
#include "CharacterData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SMMOAnimInstance.h"

ASMMOCharacterBase::ASMMOCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Set collision profile
	GetCapsuleComponent()->SetCollisionProfileName(FName("OverlapPawn"));

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxCustomMovementSpeed = 300.0f;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 200.0f;
	GetCharacterMovement()->MaxFlySpeed = 300.0f;
	GetCharacterMovement()->GravityScale = 1.0f;

	// adjust mesh location and rotation
	FVector Position;
	Position.X = 0.0f;
	Position.Y = 0.0f;
	Position.Z = -97.0f;

	FRotator Rotation;
	Rotation.Yaw = 270.0f;
	Rotation.Pitch = 0.0f;
	Rotation.Roll = 0.0f;

	GetMesh()->SetWorldLocationAndRotation(Position, Rotation);
}

void ASMMOCharacterBase::UpdateCharacterMesh(int TypeNum)
{
	USMMOGameInstance* GameInstance = Cast<USMMOGameInstance>(GetGameInstance());
	if (IsValid(GameInstance))
	{
		FCharacterData* Data = GameInstance->GetCharacterDataByNum(TypeNum);
		if (Data != nullptr)
		{
			// Set SkeletalMesh
			GetMesh()->SetSkeletalMesh(Data->Mesh);

			// Set AnimInstance
			GetMesh()->SetAnimClass(Data->AnimInstance);

			// Set Montage
			WavingMontage = Data->WavingMontage;
		}
	}
}

void ASMMOCharacterBase::Jump()
{
	Super::Jump();
	JumpButtonDown = CanJump();
}

void ASMMOCharacterBase::StopJumping()
{
	Super::StopJumping();
	JumpButtonDown = false;
}

// Called when the game starts or when spawned
void ASMMOCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASMMOCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASMMOCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

