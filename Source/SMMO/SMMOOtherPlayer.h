#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SMMOCharacterBase.h"
#include "SMMOOtherPlayer.generated.h"

UCLASS()
class SMMO_API ASMMOOtherPlayer : public ASMMOCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASMMOOtherPlayer();

	// velocity getting from server
	FVector PlayerVelocity;

	// character ui
	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* UWCharacterBarWidget;

	// set character ui
	void SetCharacterName(FText Text);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
