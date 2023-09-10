#include "SMMOOtherPlayer.h"
#include "UWCharacterInfoBar.h"
#include "Components/WidgetComponent.h"

// Sets default values
ASMMOOtherPlayer::ASMMOOtherPlayer()
{
	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UWCharacterBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("CharacterInfoBarWidget"));
	UWCharacterBarWidget->SetupAttachment(GetMesh());
	UWCharacterBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 220.0f));
	UWCharacterBarWidget->SetWidgetSpace(EWidgetSpace::World);

	static ConstructorHelpers::FClassFinder<UUWCharacterInfoBar> UI_HUD(TEXT("/Game/Blueprint/UWCharacterBar"));
	if (UI_HUD.Succeeded())
	{ 
		UWCharacterBarWidget->SetWidgetClass(UI_HUD.Class);
		UWCharacterBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}
}

void ASMMOOtherPlayer::SetCharacterName(FText Text)
{
	UUWCharacterInfoBar* uw = Cast<UUWCharacterInfoBar>(UWCharacterBarWidget->GetWidget());
	if (::IsValid(uw))
	{
		uw->SetCharacterNameText(Text);
	}
}

// Called when the game starts or when spawned
void ASMMOOtherPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASMMOOtherPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// move
	AddMovementInput(PlayerVelocity *  300.0f, DeltaTime);
}

// Called to bind functionality to input
void ASMMOOtherPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
