// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectile.h"
#include "STeleportProjectile.generated.h"

//class UParticleSystemComponent;

UCLASS()
class ROGUEACTION_API ASTeleportProjectile : public ASProjectile
{
	GENERATED_BODY()

public:
	ASTeleportProjectile();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* ExplodeComp;


	UFUNCTION()
	void HitSphere(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle_Explode;
	FTimerHandle Timer_Teleport;

	virtual void PostInitializeComponents() override;

	void ProjectileExplode();
	void Teleport();
};
