// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"

#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"


ASTeleportProjectile::ASTeleportProjectile()
{
	ExplodeComp= CreateDefaultSubobject<UParticleSystemComponent>("ExplodeEffect");
	ExplodeComp->bAutoActivate = false;
	ExplodeComp->SetupAttachment(SphereComp);


}



void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true );

	GetWorldTimerManager().SetTimer(TimerHandle_Explode, this, &ASTeleportProjectile::ProjectileExplode, 0.2f );

	
}

void ASTeleportProjectile::ProjectileExplode()
{

	MovementComp->StopMovementImmediately();

	ExplodeComp->Activate();

	GetWorldTimerManager().SetTimer(Timer_Teleport, this, &ASTeleportProjectile::Teleport, 0.2f);


}

void ASTeleportProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentHit.AddDynamic(this, &ASTeleportProjectile::HitSphere);
}


void ASTeleportProjectile::HitSphere(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	GetWorldTimerManager().ClearTimer(TimerHandle_Explode); 

	ProjectileExplode();

}


void ASTeleportProjectile::Teleport()
{

	FRotator PlayerRot = GetInstigator()->GetControlRotation();
	PlayerRot.Pitch = 0.0f;
	PlayerRot.Roll = 0.0f;

	if (GetInstigator()->TeleportTo(SphereComp->GetRelativeLocation(), PlayerRot,false, true ))
	{
		GetWorld()->DestroyActor(this);  //Remove if...when working as intended (which it may be)...
	}

	

}
