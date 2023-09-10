// Fill out your copyright notice in the Description page of Project Settings.

#include "SMMOMannequinActor.h"

// Sets default values
ASMMOMannequinActor::ASMMOMannequinActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("skeletalMesh"));
	SetRootComponent(SkeletalMeshComp);
}

// Called when the game starts or when spawned
void ASMMOMannequinActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASMMOMannequinActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASMMOMannequinActor::SetSkeletalMesh(USkeletalMesh* SkeletalMesh)
{
	SkeletalMeshComp->SetSkeletalMesh(SkeletalMesh);
}

void ASMMOMannequinActor::SetAnimInstanceClass(TSubclassOf<UAnimInstance> AnimInstanceClass)
{
	SkeletalMeshComp->SetAnimClass(AnimInstanceClass);
}

