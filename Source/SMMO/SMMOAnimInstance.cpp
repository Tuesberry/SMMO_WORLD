// Fill out your copyright notice in the Description page of Project Settings.

#include "SMMOAnimInstance.h"
#include "SMMOCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

USMMOAnimInstance::USMMOAnimInstance()
	: Speed(0.0f)
	, bIsInAir(false)
{
	
}

void USMMOAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	
	if (Cast<ACharacter>(Pawn) == nullptr)
	{
		return;
	}

	if (::IsValid(Pawn))
	{
		// set speed
		Speed = Pawn->GetVelocity().Size();

		// is in air?
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			bIsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

