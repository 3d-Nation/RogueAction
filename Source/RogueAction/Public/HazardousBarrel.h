// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HazardousBarrel.generated.h"

class UStaticMeshComponent;
class URadialForceComponent;

UCLASS()
class ROGUEACTION_API AHazardousBarrel : public AActor
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	AHazardousBarrel();


protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* ForceComp;

	//included in instructor solution...
	virtual void PostInitializeComponents() override;

	//This must be marked with UFUNCTION in order to "bind" to event
	UFUNCTION()
	void HitBarrel( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
